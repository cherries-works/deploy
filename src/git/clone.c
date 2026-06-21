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

void clone(Deploy d) {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME enviroment variable...");
        return;
    }

    char path[ONE_KB];
    sprintf(path, "%s/%s/%s/%s", home, R_CHERRIES_FOLDER_DEPLOY, d.name, d.head);
    DIR *dir = opendir(path);
    if(dir) {
        closedir(dir);
        char secondPath[ONE_KB];
        sprintf(secondPath, "%s/.git", path);
        dir = opendir(secondPath);
        if(dir) {
            closedir(dir);
            printf("✓ Repository ready\n");
            
            // printf("> path already exists - not cloning.\n");
            return;
        }
    } else {
        mkdir(path, 0777);
    }

    char buffer[ONE_KB];
    sprintf(
        buffer,
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

    sprintf(
        buffer,
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
    printf("✓ Repository cloned\n");
    return;
}