#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "utils.h"
#include "process.h"

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
