#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>

#include "git.h"
#include "config.h"
#include "utils.h"

void build(Deploy d) {
    char buffer[ONE_KB];
    sprintf(
        buffer,
        "cd %s "
        "&& "
        "cd %s "
        "&& "
        "cd %s "
        "&& "
        "(%s) > /dev/null 2>&1",
        CHERRIES_FOLDER_DEPLOY,
        d.name,
        d.head,
        d.build
    );

    system(buffer);
    printf("✓ Build completed\n");
    return;
}