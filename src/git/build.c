#include <stdio.h>
#include <stdlib.h>

#include "git.h"
#include "config.h"
#include "utils.h"

void build(Deploy d) {
    char buffer[BUFFER_ONE_KB];
    snprintf(
        buffer,
        BUFFER_ONE_KB,
        "cd %s/projects/%s/%s "
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