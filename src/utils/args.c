#include <string.h>

#include "utils.h"

void parseArgs(int argc, char* argv[], Args *args) {
    args->config = "cherries-deploy.toml";
    args->help = 0;

    for(int i = 0; i < argc; i++) {
        char* arg = argv[i];

        if(strcmp(arg, "--config") == 0) {
            if(i == argc - 1) continue;
            args->config = argv[i + 1];
        }

        if(strcmp(arg, "--help") == 0) {
            args->help = 1;
            break;
        }
    }
}
