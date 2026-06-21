#include <stdio.h>

#include "render.h"
#include "config.h"

void render() {
    printf("%s%sCherries Deploy%s ───────────────────────────────────── v0.1.0 ──── \n\n", BOLD, RED, RESET);
}

void renderStatus(Deploy d, Status s) {}

void help() {
    printf("%s%sCherries Deploy%s ───────────────────────────────────── v0.1.0 ──── \n\n", BOLD, RED, RESET);
    printf("     %-20s %-20s\n", "--config [path]", "Specify the location of config file.");
    printf("     %-20s %-20s\n\n", "--help", "Prints this.");
}
