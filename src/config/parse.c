#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "utils.h"

int isDigit(char ch) {
    return (ch >= '0' && ch <= '9') ? 1 : 0;
}

int parseWait(char *wait) {
    int size = strlen(wait);

    int total_time = 0;
    int saved_time = 0;
    for(int i = 0; i < size; i++) {
        char w = wait[i];
        if(w == ' ') continue;
        if(isDigit(w) == 1) {
            if(saved_time > 0) {
                saved_time *= 10;
            }

            saved_time += (w - '0');
        } else {
            if(w == 's') {
                total_time += saved_time;
            } else if(w == 'm') {
                total_time += saved_time * 60;
            } else if(w == 'h') {
                total_time += saved_time * 60 * 60;
            } else if(w == 'd') {
                total_time += saved_time * 60 * 60 * 24;
            }

            saved_time = 0;
            while(wait[i] != EOF && wait[i] != '\0' && wait[i] != ' ' && isDigit(wait[i]) != 1) {
                i++;
            }
        }
    }

    return total_time;
}

void parseHead(Deploy d, char* dest) {
    char buffer[ONE_KB];
    sprintf(buffer, "git ls-remote %s %s", d.repo, d.branch);
    FILE *file = popen(
        buffer,
        "r"
    );

    fscanf(file, "%40s", dest);
    pclose(file);
}

Deploy parseConfig(char *path) {
    Deploy d = {
        .repo = "",
        .branch = "",
        .name = "",
        .build = "",
        .run = "",
        
        .head = "",
        .previousHead = "",
        .failedHead = "",

        .upgrade = 1,
        .prune = 0,
        .wait = 60
    };

    char buffer[ONE_KB];
    FILE *file = fopen(path, "r");
    if(file == NULL) {
        printf("No config file found. (%s)\n", path);
        exit(EXIT_FAILURE);
    }

    size_t n = fread(buffer, 1, ONE_KB - 1, file);
    buffer[n] = '\0';

    char *line = strtok(buffer, "\n");
    while(line) {
        char *space = strchr(line, ' ');
        if(space == NULL) {
            line = strtok(NULL, "\n");
            continue;
        }

        *space = '\0';
        trim(line);

        if(strcmp(line, "repo") == 0) {
            line = space + 1;    
            space = strchr(line, ' ');
            if(space == NULL) {
                line = strtok(NULL, "\n");
                continue;
            }

            line = space + 1;
            strcpy(d.repo, line);

            char *slash = strrchr(line, '/');
            *slash = '\0';
            line = slash + 1;
            strcpy(d.name, line);
        } else if(strcmp(line, "branch") == 0) {
            line = space + 1;

            space = strchr(line, ' ');
            if(space == NULL) {
                line = strtok(NULL, "\n");
                continue;
            }

            line = space + 1;
            strcpy(d.branch, line);
        } else if(strcmp(line, "build") == 0) {
            line = space + 1;
            space = strchr(line, ' ');
            if(space == NULL) {
                line = strtok(NULL, "\n");
                continue;
            }

            line = space + 1;
            strcpy(d.build, line);
        } else if(strcmp(line, "head") == 0) {
            line = space + 1;
            space = strchr(line, ' ');
            if(space == NULL) {
                line = strtok(NULL, "\n");
                continue;
            }

            line = space + 1;
            strcpy(d.head, line);
        } else if(strcmp(line, "run") == 0) {
            line = space + 1;
            space = strchr(line, ' ');
            if(space == NULL) {
                line = strtok(NULL, "\n");
                continue;
            }

            line = space + 1;
            strcpy(d.run, line);
        } else if(strcmp(line, "upgrade") == 0) {
            line = space + 1;
            space = strchr(line, ' ');
            if(space == NULL) {
                line = strtok(NULL, "\n");
                continue;
            }

            line = space + 1;
            if(strcmp(line, "false") == 0) {
                d.upgrade = 0;
            }
        } else if(strcmp(line, "prune") == 0) {
            line = space + 1;
            space = strchr(line, ' ');
            if(space == NULL) {
                line = strtok(NULL, "\n");
                continue;
            }

            line = space + 1;
            if(strcmp(line, "true") == 0) {
                d.prune = 1;
            }
        } else if(strcmp(line, "wait") == 0) {
            line = space + 1;
            space = strchr(line, ' ');
            if(space == NULL) {
                line = strtok(NULL, "\n");
                continue;
            }

            line = space + 1;
            int w = parseWait(line);
            d.wait = w;
        }

        line = strtok(NULL, "\n");
    }

    fclose(file);
    return d;
}
