#include "commands.h"
#include "process.h"

void parseCommand(
    Commands command,
    Args args
) {
    if(command == RUN) {
        runner(args);
    }
    if(command == HELP) {
        help();
    }
    if(command == PS) {
        list();
    }
    if(command == STOP) {
        stopAll();
    }
}
