#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>

#include "config.h"
#include "process.h"
#include "utils.h"
#include "setup.h"
#include "render.h"

volatile sig_atomic_t child_pid = -1;
void handle_sigint(int sig) {
    if (child_pid > 0) {
        killpg(child_pid, SIGTERM);
    }

    _exit(0);
}

struct DeployArgs {
    int help;
    char *config;
};

struct DeployArgs parseArgs(int argc, char* argv[]) {
    struct DeployArgs p = {
        .help = 0,
        .config = "cherries-deploy.toml"
    };

    for(int i = 0; i < argc; i++) {
        char* arg = argv[i];

        if(strcmp(arg, "--config") == 0) {
            if(i == argc - 1) continue;
            p.config = argv[i + 1];
        }

        if(strcmp(arg, "--help") == 0) {
            p.help = 1;
            break;
        }
    }


    return p;
}


int main(int argc, char* argv[]) {
    struct DeployArgs args = parseArgs(argc, argv);
    if(args.help == 1) {
        help();
        return 0;
    }

    signal(SIGINT, handle_sigint);
    render();
    setup();

    struct Status s = {
        .status = idle,
        .pid = 0,

        .terminalLines = 0,
        .started = 0,
        .lastCheck = 0,
        .failedCommitCheck = 0
    };

    struct Deploy d = parseConfig(args.config);
    setupDeploy(&d);

    char previousHead[ONE_KB];
    char head[ONE_KB];

    child_pid = start(d, &s);
    if(child_pid == -1) {
        printf("× Application failed (exiting)\n");
        exit(EXIT_FAILURE);
    }
    
    printf("✓ Application running (pid %d)\n\n", child_pid);
    s.status = waiting;

    while(1) {
        int status;
        pid_t r = waitpid(child_pid, &status, WNOHANG);

        if (r == child_pid) {
            s.latestCommitCheck = 0;
            s.failedCommitCheck = 0;
            if (WIFEXITED(status)) {
                printf("× Application exited (exited)\n");
            } else if (WIFSIGNALED(status)) {
                printf("× Application terminated (terminated)\n");
            } else {
                printf("× Application failed (restarting)\n");
            }

            printf("▲ Restarting\n");
            child_pid = restart(d, &s);
            if(child_pid == -1) {
                printf("× Application failed (exited)\n");
                exit(EXIT_FAILURE);
            }
            printf("✓ Application restarted (pid %d)\n\n", child_pid);
        }

        parseHead(d, head);
        if(strcmp(d.head, head) != 0 && d.upgrade == 1) {
            if(strcmp(d.failedHead, head) == 0) {
                if(s.failedCommitCheck == 0) {
                    s.failedCommitCheck = 1;
                }
            } else {
                s.latestCommitCheck = 0;
                printf("▲ New commit detected\n");
                printf("▲ Updating program\n");

                if(strlen(d.previousHead) > 0) {
                    strcpy(previousHead, d.previousHead);
                }

                strcpy(d.previousHead, d.head);
                strcpy(d.head, head);

                kill(child_pid, SIGKILL);
                waitpid(child_pid, NULL, 0);

                printf("▲ Stopped application (pid %d)\n\n", child_pid);
                child_pid = restart(d, &s);

                if(child_pid == -1) {
                    printf("× Application failed (rolling back)\n\n");
                    child_pid = rollback(&d, d.previousHead);
                    if(child_pid == -1) {
                        printf("× Application failed (exited)\n");
                        exit(EXIT_FAILURE);
                    } else {
                        printf("✓ Rollback completed\n");
                    }
                }

                s.failedCommitCheck = 0;
                printf("✓ Application running (pid %d)\n\n", child_pid);
                if(d.prune == 1 && strlen(previousHead) != 0) {
                    char previousHeadPath[ONE_KB];
                    setupPathHash(d, previousHead, previousHeadPath);
                    cleanDir(previousHeadPath);
                }
            }
        }

        if(s.latestCommitCheck == 0) {
            s.latestCommitCheck = 1;
            printf("▲ Waiting for next commit\n\n");
        }
        sleep(d.wait);
    }

    kill(child_pid, SIGKILL);
    return 0;
}
