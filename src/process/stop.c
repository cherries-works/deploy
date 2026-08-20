#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>

#include "git.h"
#include "config.h"
#include "utils.h"

void stop(pid_t pid) {
    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);

    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME variable set.\n");
        return;
    }

    char path[BUFFER_ONE_KB];
    snprintf(
        path,
        BUFFER_ONE_KB,
        "%s/%s/running/%d",
        home,
        R_CHERRIES_FOLDER_DEPLOY,
        pid
    );
    remove(path);

    return;
}
