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
#include "commands.h"

int main(int argc, char* argv[]) {
    Args args = {};
    parseArgs(argc, argv, &args);
    parseCommand(args.command, args);

    return 0;
}
