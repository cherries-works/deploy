#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

#include "git.h"
#include "config.h"
#include "process.h"
#include "setup.h"

const int EVENT_SIZE = 5;
void eventAppend(Status *s, char *_event) {
    if (s->event_index < EVENT_SIZE) {
        strcpy(s->events[s->event_index], _event);
        s->event_index++;
        return;
    }

    for (int i = 0; i < (EVENT_SIZE - 1); i++) {
        strcpy(s->events[i], s->events[i + 1]);
    }

    strcpy(s->events[(EVENT_SIZE - 1)], _event);
}