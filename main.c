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

typedef struct {
    int help;
    char *config;
} DeployArgs;

DeployArgs parseArgs(int argc, char* argv[]) {
    DeployArgs p = {
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
    DeployArgs args = parseArgs(argc, argv);
    if(args.help == 1) {
        help();
        return 0;
    }

    signal(SIGINT, handle_sigint);
    render();
    setup();

    Status s = {
        .status = idle,
        .pid = 0,

        .started = 0,
        .failed_commit_check = 0
    };

    Deploy d = parseConfig(args.config);
    setupDeploy(&d);

    char previous_head[BUFFER_ONE_KB];
    char head[BUFFER_ONE_KB];

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
            s.latest_commit_check = 0;
            s.failed_commit_check = 0;
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
            if(strcmp(d.failed_head, head) == 0) {
                if(s.failed_commit_check == 0) {
                    s.failed_commit_check = 1;
                }
            } else {
                s.latest_commit_check = 0;
                printf("▲ New commit detected\n");
                printf("▲ Updating program\n");

                if(strlen(d.previous_head) > 0) {
                    strcpy(previous_head, d.previous_head);
                }

                strcpy(d.previous_head, d.head);
                strcpy(d.head, head);

                kill(child_pid, SIGKILL);
                waitpid(child_pid, NULL, 0);

                printf("▲ Stopped application (pid %d)\n\n", child_pid);
                child_pid = restart(d, &s);

                if(child_pid == -1) {
                    printf("× Application failed (rolling back)\n\n");
                    child_pid = rollback(&d, d.previous_head);
                    if(child_pid == -1) {
                        printf("× Application failed (exited)\n");
                        exit(EXIT_FAILURE);
                    } else {
                        printf("✓ Rollback completed\n");
                    }
                }

                s.failed_commit_check = 0;
                printf("✓ Application running (pid %d)\n\n", child_pid);
                if(d.prune == 1 && strlen(previous_head) != 0) {
                    char previous_head_path[BUFFER_ONE_KB];
                    setupPathHash(d, previous_head, previous_head_path, BUFFER_ONE_KB);
                    cleanDir(previous_head_path);
                }
            }
        }

        if(s.latest_commit_check == 0) {
            s.latest_commit_check = 1;
            printf("▲ Waiting for next commit\n\n");
        }
        sleep(d.wait);
    }

    kill(child_pid, SIGKILL);
    return 0;
}
