#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>

typedef enum {
    idle,
    waiting,
    building,
    running,
    cloning,
    deploying
} STATUS;

typedef struct {
    int started;
    int lastCheck;

    int terminalLines;

    // whether or not we already checked a failed commit
    int failedCommitCheck;

    // whether or not we already checked for a commit
    int latestCommitCheck;

    STATUS status;
    pid_t pid;
} Status;

typedef struct {
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
} Deploy;

extern Deploy parseConfig(char *path);
extern void parseHead(Deploy d, char* dest);

#endif
