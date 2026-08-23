#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "render.h"
#include "config.h"
#include "utils.h"

const char* status_list[] = {
    "idle",
    "parsing",
    "waiting",
    "building",
    "running",
    "cloning",
    "deploying"
};

void render(Status *s) {
    pid_t pid = fork();
    if (pid < 0) {
        printf("Error: Forking renderer failed.\n");
        return;
    }

    if(pid == 0) {
        while(true) {
            clearLines((unsigned)s->previous_terminal_length);

            s->previous_terminal_length = 0;
            printf("%s%sCherries Deploy%s ──────────────────────────── v0.1.0 ──────┐\n", BOLD, RED, RESET);
            printf("Press 'd' to detach | Press 'q' to quit\n\n");
            
            s->previous_terminal_length += 3;

            printf("%s┌─────────────────────────────────────────────────────────┐ %s\n", BOLD, RESET);
            printf("%s│ PID %-51d │%s\n", BOLD, s->pid, RESET);
            printf("%s│ STATUS %-48s │%s\n", BOLD, status_list[s->status], RESET);
            printf("%s│ HASH %-50s │%s\n", BOLD, s->hash, RESET);
            
            char formatted_time[256];
            formatTimeHumanReadable((unsigned)s->timer, formatted_time, 256);
            printf("%s│ TIME %-50s │%s\n", BOLD, formatted_time, RESET);
            printf("%s└─────────────────────────────────────────────────────────┘ %s\n", BOLD, RESET);
            printf("%s┌─────────────────────────────────────────────────────────┐ %s\n", BOLD, RESET);
            printf("%s│ EVENTS: %-52s│\n", BOLD, RESET);

            s->previous_terminal_length += 8;

            for(int i = 0; i < s->event_index; i++) {
                printf("%s│ %-56s│%s\n", BOLD, s->events[i], RESET);
                s->previous_terminal_length++;
            }
            printf("%s└─────────────────────────────────────────────────────────┘ %s\n", BOLD, RESET);
            s->previous_terminal_length++;
            s->timer++;
            sleep(1);
        }
    }
    s->render_pid = pid;
}
