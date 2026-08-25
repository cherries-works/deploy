#ifndef PROCESS_H
#define PROCESS_H

#include "utils.h"
#include <sys/types.h>
#include <pthread.h>

struct shmbuf {
    pthread_mutex_t lock;
    Status status;
};

extern pid_t rollback(Deploy *d, char *hash);
extern pid_t start(Deploy d, Status *s);
extern pid_t restart(Deploy d, Status *s);
extern pid_t run(Deploy d);
extern pid_t initialize(
    Status *status,
    Args *args
);
extern void stop(pid_t pid);
extern void eventAppend(Status *s, char *_event);

#endif
