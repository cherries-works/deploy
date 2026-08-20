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

pid_t initialize(
    int argc,
    char *argv[],

    Deploy *deploy,
    Status *status,
    Args *args
) {
    setup();

    parseArgs(argc, argv, args);
    if(args->help == true) return 1;

    status->status = parsing;
    parseConfig(args->config, deploy);

    status->status = deploying;
    setupDeploy(deploy);

    pid_t child_pid = start(*deploy, status);
    if(child_pid == -1) {
        printf("× Application failed (exited)\n");
        exit(EXIT_FAILURE);
    }
    
    status->status = waiting;
    return child_pid;
}
