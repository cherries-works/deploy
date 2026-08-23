#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>

#include "config.h"
#include "utils.h"
#include "process.h"

void clone(Deploy d, Status *status) {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME enviroment variable...");
        return;
    }

    char path[BUFFER_ONE_KB];
    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s/projects/%s/%s",
        home,
        R_CHERRIES_FOLDER_DEPLOY,
        d.name,
        d.head
    );

    DIR *dir = opendir(path);
    if(dir) {
        closedir(dir);
        char secondPath[BUFFER_ONE_KB];
        snprintf(secondPath, BUFFER_ONE_KB, "%s/.git", path);
        dir = opendir(secondPath);
        if(dir) {
            closedir(dir);
            eventAppend(status, "+ Repository ready");
            return;
        }
    } else {
        mkdir(path, 0777);
    }

    char buffer[BUFFER_ONE_KB];
    snprintf(
        buffer,
        BUFFER_ONE_KB,
        "cd %s " 
        "&& "
        "git clone " 
        "--depth 1 "
        "--quiet "
        "--branch %s "
        "%s .",
        path,
        d.branch,
        d.repo
    );
    system(buffer);

    snprintf(
        buffer,
        BUFFER_ONE_KB,
        "cd %s " 
        "&& "
        "git fetch " 
        "--quiet " 
        "origin "
        "%s "
        "&& "
        "git checkout " 
        "--quiet "
        "%s",
        path,
        d.head,
        d.head
    );
    system(buffer);

    eventAppend(status, "+ Repository cloned");
    return;
}