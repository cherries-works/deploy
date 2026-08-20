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

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);

    Status status = {};
    Deploy deploy = {};
    Args args = {};
    render(status);

    pid_t child_pid = initialize(
        argc,
        argv,
        &deploy,
        &status,
        &args
    );

    if(args.help == 1) {
        help();
        return 0;
    }

    char previous_head[BUFFER_ONE_KB];
    char head[BUFFER_ONE_KB];

    status.status = waiting;

    while(true) {
        int pid_status;
        pid_t r = waitpid(child_pid, &pid_status, WNOHANG);

        if (r == child_pid) {
            status.latest_commit_check = false;
            status.failed_commit_check = false;
            if (WIFEXITED(pid_status)) {
                printf("× Application exited (exited)\n");
            } else if (WIFSIGNALED(pid_status)) {
                printf("× Application terminated (terminated)\n");
            } else {
                printf("× Application failed (restarting)\n");
            }

            printf("▲ Restarting\n");
            status.status = building;

            child_pid = restart(deploy, &status);
            if(child_pid == -1) {
                printf("× Application failed (exited)\n");
                exit(EXIT_FAILURE);
            }
            printf("✓ Application restarted (pid %d)\n\n", child_pid);
            status.status = idle;
        }

        parseHead(deploy, head);
        if(strcmp(deploy.head, head) != 0 && deploy.upgrade) {
            if(strcmp(deploy.failed_head, head) == false) {
                if(status.failed_commit_check == false) {
                    status.failed_commit_check = true;
                }
            } else {
                status.status = building;

                status.latest_commit_check = false;
                printf("▲ New commit detected\n");
                printf("▲ Updating program\n");

                if(strlen(deploy.previous_head) > 0) {
                    strcpy(previous_head, deploy.previous_head);
                }

                strcpy(deploy.previous_head, deploy.head);
                strcpy(deploy.head, head);

                kill(child_pid, SIGKILL);
                waitpid(child_pid, NULL, 0);

                printf("▲ Stopped application (pid %d)\n\n", child_pid);
                child_pid = restart(deploy, &status);

                if(child_pid == -1) {
                    printf("× Application failed (rolling back)\n\n");

                    child_pid = rollback(&deploy, deploy.previous_head);
                    if(child_pid == -1) {
                        printf("× Application failed (exited)\n");
                        exit(EXIT_FAILURE);
                    } else {
                        status.status = deploying;
                        printf("✓ Rollback completed\n");
                    }
                }

                status.failed_commit_check = false;
                printf("✓ Application running (pid %d)\n\n", child_pid);
                status.status = deploying;

                if(deploy.prune && strlen(previous_head) != 0) {
                    char previous_head_path[BUFFER_ONE_KB];
                    setupPathHash(deploy, previous_head, previous_head_path, BUFFER_ONE_KB);
                    cleanDir(previous_head_path);
                }
            }
        }

        if(!status.latest_commit_check) {
            status.latest_commit_check = true;
            printf("▲ Waiting for next commit\n\n");
        }

        status.status = idle;
        sleep(deploy.wait);
    }

    kill(child_pid, SIGKILL);
    return 0;
}
