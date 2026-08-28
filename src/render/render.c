#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "render.h"
#include "config.h"
#include "utils.h"
#include "process.h"
#include "log.h"

const char* status_list[] = {
    "idle",
    "parsing",
    "waiting",
    "building",
    "running",
    "cloning",
    "deploying"
};

pid_t render() {
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Forking renderer failed.\n");
        return -1;
    }

    
    if(pid == 0) {
        int shm_fd = shm_open(CHERRIES_DEPLOY_SHM, O_RDWR, 0600);
        if (shm_fd == -1) {
            exit(EXIT_FAILURE);
        }
    
        ftruncate(shm_fd, sizeof(struct shmbuf));
        struct shmbuf *shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (shmp == MAP_FAILED) {
            _log(
                L_ERROR,
                "Mapping object failed"
            );
            exit(EXIT_FAILURE);
        }
        shmp->status.render_pid = getpid();

        while(true) {
            clearLines((unsigned)shmp->status.previous_terminal_length);

            shmp->status.previous_terminal_length = 0;
            printf("%s%sCherries Deploy%s ──────────────────────────── v0.2.1 ──────┐\n", BOLD, RED, RESET);
            printf("Press 'd' to detach | Press 'q' to quit\n\n");
            
            shmp->status.previous_terminal_length += 3;

            printf("%s┌─────────────────────────────────────────────────────────┐ %s\n", BOLD, RESET);
            printf("%s│ PID %-51d │%s\n", BOLD, shmp->status.pid, RESET);
            printf("%s│ STATUS %-48s │%s\n", BOLD, status_list[shmp->status.status], RESET);
            printf("%s│ HASH %-50s │%s\n", BOLD, shmp->status.hash, RESET);
            
            char formatted_time[256];
            formatTimeHumanReadable((unsigned)shmp->status.timer, formatted_time, 256);
            printf("%s│ TIME %-50s │%s\n", BOLD, formatted_time, RESET);
            printf("%s└─────────────────────────────────────────────────────────┘ %s\n", BOLD, RESET);
            printf("%s┌─────────────────────────────────────────────────────────┐ %s\n", BOLD, RESET);
            printf("%s│ EVENTS: %-52s│\n", BOLD, RESET);

            shmp->status.previous_terminal_length += 8;

            for(int i = 0; i < shmp->status.event_index; i++) {
                printf("%s│ %-56s│%s\n", BOLD, shmp->status.events[i], RESET);
                shmp->status.previous_terminal_length++;
            }
            printf("%s└─────────────────────────────────────────────────────────┘ %s\n", BOLD, RESET);
            shmp->status.previous_terminal_length++;
            shmp->status.timer++;
            sleep(1);
        }
    }

    return pid;
}
