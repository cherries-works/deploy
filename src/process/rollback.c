#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

#include "git.h"
#include "config.h"
#include "process.h"

pid_t rollback(struct Deploy *d, char *hash) {
    strcpy(d->failedHead, d->head);
    strcpy(d->head, hash);

    pid_t pid = run(*d);
    return pid;
}
