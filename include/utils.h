#ifndef UTILS_H
#define UTILS_H

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
