#ifndef UTILS_H
#define UTILS_H

#include "config.h"

typedef struct {
    int help;
    char *config;
} Args;

extern void parseArgs(int argc, char* argv[], Args *args);

extern pid_t initialize(
    int argc,
    char *argv[],
    Deploy *d,
    Status *s,
    Args *args
);

extern void trim(char *buffer);
extern void cleanDir(char *path);
extern void clearLine();
extern void clearLines(unsigned i);

extern const size_t BUFFER_ONE_KB;
extern const char SPACE_IN_ASCII;

extern const char* CHERRIES_FOLDER;
extern const char* CHERRIES_FOLDER_DEPLOY;
extern const char* R_CHERRIES_FOLDER;
extern const char* R_CHERRIES_FOLDER_DEPLOY;

#endif
