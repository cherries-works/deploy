#include <stdio.h>
#include "render.h"

void help() {
    printf("%s%sCherries Deploy%s ───────────────────────────────────── v0.2.0 ──── \n", BOLD, RED, RESET);
    printf("     %-20s %-20s\n", "--config [path]", "Specify the location of config file.");
    printf("     %-20s %-20s\n\n", "--help", "Prints this.");
}
