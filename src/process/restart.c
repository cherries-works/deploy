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

pid_t restart(Deploy d, Status *s) {
    setupDeploy(&d);

    pid_t pid = start(d, s);
    return pid;
}
