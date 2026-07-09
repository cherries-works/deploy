#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>

#include "config.h"
#include "utils.h"

void setup() {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME enviroment variable...");
        return;
    }

    char path[BUFFER_ONE_KB];
    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s",
        home,
        R_CHERRIES_FOLDER
    );

    DIR *dir = opendir(path);
    if(!dir) {
        mkdir(path, 0755);
    } else {
        closedir(dir);
    }

    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s",
        home,
        R_CHERRIES_FOLDER_DEPLOY
    );

    dir = opendir(path);
    if(!dir) {
        mkdir(path, 0755);
    } else {
        closedir(dir);
    }

    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s/projects",
        home,
        R_CHERRIES_FOLDER_DEPLOY
    );

    dir = opendir(path);
    if(!dir) {
        mkdir(path, 0755);
    } else {
        closedir(dir);
    }

    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s/running",
        home,
        R_CHERRIES_FOLDER_DEPLOY
    );
    
    dir = opendir(path);
    if(!dir) {
        mkdir(path, 0755);
    } else {
        closedir(dir);
    }
}

void setupPath(
    Deploy d,
    char *buffer,
    size_t buffer_size
) {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME enviroment variable...");
        return;
    }

    snprintf(
        buffer,
        buffer_size,
        "%s/%s/projects/%s/%s",
        home,
        R_CHERRIES_FOLDER_DEPLOY,
        d.name,
        d.head
    );
    return;
}

void setupPathHash(
    Deploy d,
    char *hash,
    char *buffer,
    size_t buffer_size
) {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME enviroment variable...");
        return;
    }

    snprintf(
        buffer,
        buffer_size,
        "%s/%s/projects/%s/%s",
        home,
        R_CHERRIES_FOLDER_DEPLOY,
        d.name,
        hash
    );
    return;
}

void setupDeploy(Deploy *d) {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME enviroment variable...");
        return;
    }

    char path[BUFFER_ONE_KB];
    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s/projects/%s",
        home,
        R_CHERRIES_FOLDER_DEPLOY,
        d->name
    );

    DIR *dir = opendir(path);
    if(!dir) {
        mkdir(path, 0755);
    } else {
        closedir(dir);
    }

    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s/projects/%s/%s",
        home,
        R_CHERRIES_FOLDER_DEPLOY,
        d->name,
        d->head
    );

    dir = opendir(path);
    if(!dir) {
        mkdir(path, 0755);
    } else {
        closedir(dir);
    }

    char head[BUFFER_ONE_KB];
    if(strlen(d->head) == 0) {
        parseHead(*d, head);
        strcpy(d->head, head);
    }

    return;
}