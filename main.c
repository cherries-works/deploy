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
#include "log.h"

int main(int argc, char* argv[]) {
    Args args = {};
    parseArgs(argc, argv, &args);

    setup();
    setupLog();
    switch (args.command) {
        case RUN:
            runner(args);
            break;

        case HELP:
            help();
            break;

        case PS:
            list();
            break;

        case STOP:
            stopAll();
            break;
        
        default:
            break;
    }

    return 0;
}
