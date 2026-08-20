#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>
#include <stdbool.h>

typedef enum {
    idle,
    parsing,
    waiting,
    building,
    running,
    cloning,
    deploying
} Status_e;

typedef struct {
    bool started;

    // whether or not we already checked a failed commit
    bool failed_commit_check;

    // whether or not we already checked for a commit
    bool latest_commit_check;
    
    Status_e status;
    pid_t pid;
    char hash[256];
} Status;

typedef struct {
    char repo[256];
    char branch[256];
    char name[256];
    char build[256];
    char run[256];

    char head[256];
    char previous_head[256];
    char failed_head[256];

    bool upgrade;
    bool prune;
    unsigned wait;
} Deploy;

extern void parseConfig(char *path, Deploy *d);
extern void parseHead(Deploy d, char* dest);

#endif
