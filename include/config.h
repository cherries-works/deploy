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
    bool failed_commit_check; // whether or not we already checked a failed commit
    bool latest_commit_check; // whether or not we already checked for a commit
    
    Status_e status;

    pid_t pid;
    pid_t render_pid;
    char hash[256];

    time_t timer;
    int8_t previous_terminal_length;
    
    int8_t event_index;
    char events[5][256];
} Status;

typedef struct {
    bool exists;

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
