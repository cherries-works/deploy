#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "render.h"
#include "utils.h"
#include "process.h"

void help() {
    printf("%s%sCherries Deploy%s ───────────────────────────────────── v0.2.1 ──── \n", BOLD, RED, RESET);
    printf(" > %-20s %-20s\n", "run", "Runs a deplyment (default option).");
    printf("     %s%-20s %-20s%s\n", DIM, "--config [path]", "Specify the location of config file.", RESET);
    printf(" > %-20s %-20s\n", "stop", "Stops all running processes by Deploy.");
    printf(" > %-20s %-20s\n", "help", "Prints this.");
    printf(" > %-20s %-20s\n", "ps", "Prints deployments that are currently running.");
    printf("\n");
}

void list() {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME variable set.\n");
        return;
    }

    char path[BUFFER_ONE_KB];
    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s/running",
        home,
        R_CHERRIES_FOLDER_DEPLOY
    );

    struct dirent *entry;
    DIR *dir = opendir(path);
    if(dir == NULL) return;

    while((entry = readdir(dir)) != NULL) {
        if(entry->d_type == DT_DIR) continue;
        if(strcmp(entry->d_name, ".") == 0) continue;
        if(strcmp(entry->d_name, "..") == 0) continue;

        pid_t pid = (pid_t)atoi(entry->d_name);
        printf("PID: %-5d | STATUS: RUNNING\n", pid);
    }

    return;
}

void stopAll() {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME variable set.\n");
        return;
    }

    char path[BUFFER_ONE_KB];
    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s/running",
        home,
        R_CHERRIES_FOLDER_DEPLOY
    );

    struct dirent *entry;
    DIR *dir = opendir(path);
    if(dir == NULL) return;

    while((entry = readdir(dir)) != NULL) {
        if(entry->d_type == DT_DIR) continue;
        if(strcmp(entry->d_name, ".") == 0) continue;
        if(strcmp(entry->d_name, "..") == 0) continue;

        pid_t pid = (pid_t)atoi(entry->d_name);
        stop(pid);
    }

    return;
}
