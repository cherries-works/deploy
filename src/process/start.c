#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

#include "git.h"
#include "config.h"
#include "process.h"

pid_t start(Deploy d, Status *s) {
    s->status = cloning;
    clone(d);

    s->status = building;
    build(d);
    
    s->status = running;
    pid_t pid = run(d);
    return pid;
}
