#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "git.h"
#include "config.h"
#include "utils.h"

pid_t run(struct Deploy d) {
    char *home = getenv("HOME");
    if(home == NULL) {
        printf("Error: No HOME variable set.");
        return -1;
    }

    char path[ONE_KB];
    sprintf(
        path, 
        "%s/%s/%s/%s",
        home,
        R_CHERRIES_FOLDER_DEPLOY,
        d.name,
        d.head
    );

    char buffer[ONE_KB];
    sprintf(
        buffer,
        "cd %s "
        "&& "
        "%s",
        path,
        d.run
    );

    pid_t pid = fork();
    if (pid < 0) return -1;

    if (pid == 0) {
        setpgid(0, 0);

        int dev_null = open("/dev/null", O_WRONLY);
        if (dev_null >= 0) {
            dup2(dev_null, STDOUT_FILENO);
            dup2(dev_null, STDERR_FILENO);
            close(dev_null);
        }
        
        execlp("sh", "sh", "-c", buffer, NULL);
        perror("execlp");
        _exit(EXIT_FAILURE);
    }

    sleep(1);

    int status;
    pid_t r = waitpid(pid, &status, WNOHANG);
    if(r == pid) return -1;

    return pid;
}

