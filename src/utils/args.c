#include <string.h>

#include "utils.h"

void parseArgs(int argc, char* argv[], Args *args) {
    args->config = "cherries-deploy.toml";
    args->command = RUN;

    for(int i = 0; i < argc; i++) {
        char* arg = argv[i];

        if(strcmp(arg, "--config") == 0) {
            if(i == argc - 1) continue;
            args->config = argv[i + 1];
        }

        if(strcmp(arg, "help") == 0) {
            args->command = HELP;
            break;
        }

        if(strcmp(arg, "run") == 0) {
            args->command = RUN;
            break;
        }

        if(strcmp(arg, "ps") == 0) {
            args->command = PS;
            break;
        }

        if(strcmp(arg, "ls") == 0) {
            args->command = PS;
            break;
        }

        if(strcmp(arg, "list") == 0) {
            args->command = PS;
            break;
        }

        if(strcmp(arg, "stop") == 0) {
            args->command = STOP;
            break;
        }

        if(strcmp(arg, "end") == 0) {
            args->command = STOP;
            break;
        }
    }
}
