#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>
#include <termios.h>
#include <malloc.h>
#include <sys/mman.h>
#include <poll.h>

#include "config.h"
#include "process.h"
#include "utils.h"
#include "setup.h"
#include "render.h"
#include "log.h"

pid_t startProgram(
    Deploy *deploy,
    Status *status,
    Args *args
) {
    setup();

    status->status = parsing;
    parseConfig(args->config, deploy);
    if(!deploy->exists) return -1;

    status->status = deploying;
    setupDeploy(deploy);

    pid_t program_pid = start(*deploy, status);
    if(program_pid == -1) {
        eventAppend(status, "x Application failed (exited)");
        exit(EXIT_FAILURE);
    }

    status->pid = program_pid;
    
    status->status = waiting;
    return program_pid;
}

pid_t initialize(
    Status *status,
    Args *args
) {
    Deploy deploy = {};
    pid_t main_pid = fork();
    if(main_pid == -1) return -1;

    if(main_pid == 0) {
        _log(L_INFO, "Setting up Deploy.");

        pid_t program_pid = startProgram(&deploy, status, args);
        if(program_pid == -1) return -1;
        
        char previous_head[BUFFER_ONE_KB];
        char head[BUFFER_ONE_KB];

        status->status = waiting;
        char message[64];
        while(true) {
            int pid_status;
            pid_t r = waitpid(program_pid, &pid_status, WNOHANG);
            strcpy(status->hash, deploy.head);

            if (r == program_pid) {
                status->latest_commit_check = false;
                status->failed_commit_check = false;
                if (WIFEXITED(pid_status)) {
                    eventAppend(status, "x Application failed (exited)\n");
                } else if (WIFSIGNALED(pid_status)) {
                    eventAppend(status, "x Application terminated (terminated)\n");
                } else {
                    eventAppend(status, "x Application failed (restarting)\n");
                }

                eventAppend(status, "^ Restarting\n");
                status->status = building;

                program_pid = restart(deploy, status);
                if(program_pid == -1) {
                    eventAppend(status, "x Application failed (exited)\n");
                    exit(EXIT_FAILURE);
                }

                snprintf(message, sizeof(message), "+ Application restarted (pid %d)", program_pid);
                eventAppend(status, message);
                status->status = idle;
            }

            parseHead(deploy, head);
            if(strcmp(deploy.head, head) != 0 && deploy.upgrade) {
                if(strcmp(deploy.failed_head, head) == false) {
                    if(status->failed_commit_check == false) {
                        status->failed_commit_check = true;
                    }
                } else {
                    status->status = building;
    
                    status->latest_commit_check = false;
                    eventAppend(status, "^ New commit detected");
                    eventAppend(status, "^ Updating program");
    
                    if(strlen(deploy.previous_head) > 0) {
                        strcpy(previous_head, deploy.previous_head);
                    }
    
                    strcpy(deploy.previous_head, deploy.head);
                    strcpy(deploy.head, head);
    
                    stop(program_pid);
    
                    snprintf(message, sizeof(message), "+ Stopped application (pid %d)", program_pid);
                    eventAppend(status, message);
                    program_pid = restart(deploy, status);
    
                    if(program_pid == -1) {
                        eventAppend(status, "x Application failed (rolling back)");
                        program_pid = rollback(&deploy, deploy.previous_head);
                        if(program_pid == -1) {
                            eventAppend(status, "x Application failed (exited)");
                            exit(EXIT_FAILURE);
                        } else {
                            eventAppend(status, "+ Rollback completed");
                            status->status = deploying;
                        }
                    }
    
                    status->failed_commit_check = false;
    
                    snprintf(message, sizeof(message), "+ Application running (pid %d)", program_pid);
                    eventAppend(status, message);
                    status->status = deploying;
    
                    if(deploy.prune && strlen(previous_head) != 0) {
                        char previous_head_path[BUFFER_ONE_KB];
                        setupPathHash(deploy, previous_head, previous_head_path, BUFFER_ONE_KB);
                        cleanDir(previous_head_path);
                    }
                }
            }
    
            if(!status->latest_commit_check) {
                status->latest_commit_check = true;
                eventAppend(status, "^ Waiting for next commit");
            }
    
            status->status = idle;
            sleep(deploy.wait);
        }
    }

    return main_pid;
}
