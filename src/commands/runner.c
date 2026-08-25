#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>
#include <termios.h>
#include <malloc.h>
#include <sys/mman.h>
#include <poll.h>
#include <fcntl.h>

#include "config.h"
#include "process.h"
#include "utils.h"
#include "setup.h"
#include "render.h"
#include "log.h"

volatile sig_atomic_t main_pid = -1;
volatile sig_atomic_t render_pid = -1;
struct termios oldt;
void handle_sigint(int sig) {
    if (main_pid > 0) {
        stop(main_pid);
    }
    if (render_pid > 0) {
        stop(render_pid);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    _exit(0);
}

struct pollfd pfd = {
    .fd = STDIN_FILENO,
    .events = POLLIN
};

int runner(Args args) {
    signal(SIGINT, handle_sigint);
    tcgetattr(STDIN_FILENO, &oldt);

    shm_unlink(CHERRIES_DEPLOY_SHM);
    int shm_fd = shm_open(CHERRIES_DEPLOY_SHM, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (shm_fd == -1) {
        exit(EXIT_FAILURE);
    }

    struct shmbuf *shmp;
    shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shmp == MAP_FAILED) {
        _log(
            L_ERROR,
            "Mapping object failed"
        );
        exit(EXIT_FAILURE);
    }

    main_pid = initialize(
        &shmp->status,
        &args
    );

    if(main_pid == -1) {
        stop(render_pid);
        exit(EXIT_FAILURE);
    }

    render(&shmp->status);
    render_pid = shmp->status.render_pid;
    if(render_pid == -1) {
        exit(EXIT_FAILURE);
    }

    struct termios newt;
    newt = oldt;
    newt.c_lflag &= (tcflag_t)~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    while (true) {
        int ret = poll(&pfd, 1, 1000);
        if (ret < 0) break;

        if (ret > 0 && (pfd.revents & POLLIN)) {
            char c;

            if (read(STDIN_FILENO, &c, 1) == 1) {
                if (c == 'q') {
                    break;
                }

                if (c == 'd') {
                    stop(shmp->status.render_pid);
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return 0;
                }
            }
        }
    }
    
    stop(main_pid);
    stop(shmp->status.render_pid);
    stop(shmp->status.pid);
    shm_unlink(CHERRIES_DEPLOY_SHM);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
