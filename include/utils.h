#ifndef UTILS_H
#define UTILS_H

#include "config.h"

typedef enum {
    HELP,
    RUN,
    STOP,
    PS
} Commands;

typedef struct {
    Commands command;
    char *config;
} Args;

extern void parseArgs(int argc, char* argv[], Args *args);

extern void trim(char *buffer);
extern void cleanDir(char *path);
extern void clearLine();
extern void clearLines(unsigned i);
extern void formatTimeHumanReadable(
    long unsigned seconds, 
    char* buffer,
    size_t size
);
extern void formatTime(time_t _time, char *buffer, size_t size);
extern long unsigned unformatTime(char *buffer);

extern const size_t BUFFER_ONE_KB;
extern const char SPACE_IN_ASCII;

extern const char* CHERRIES_FOLDER;
extern const char* CHERRIES_FOLDER_DEPLOY;
extern const char* R_CHERRIES_FOLDER;
extern const char* R_CHERRIES_FOLDER_DEPLOY;
extern const char* CHERRIES_DEPLOY_SHM;

#endif