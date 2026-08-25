#ifndef COMMANDS_H
#define COMMANDS_H

#include "utils.h"

void help();
void list();
void stopAll();
void runner(Args args);
void parseCommand(
    Commands command,
    Args args
);

#endif