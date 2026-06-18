#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>

enum STATUS {
    idle,
    waiting,
    building,
    running,
    cloning,
    deploying
};

struct Status {
    int started;
    int lastCheck;

    int terminalLines;

    // whether or not we already checked a failed commit
    int failedCommitCheck;

    // whether or not we already checked for a commit
    int latestCommitCheck;

    enum STATUS status;
    pid_t pid;
};

struct Deploy {
    char repo[256];
    char branch[256];
    char name[256];
    char build[256];
    char run[256];

    char head[256];
    char previousHead[256];
    char failedHead[256];

    int upgrade;
    int prune;
    int wait;
};

extern struct Deploy parseConfig(char *path);
extern void parseHead(struct Deploy d, char* dest);

#endif
