#ifndef SETUP_H
#define SETUP_H

#include "config.h"

extern void setup();
extern void setupDeploy(struct Deploy *d);
extern void setupPath(struct Deploy d, char *buffer);
extern void setupPathHash(struct Deploy d, char *hash, char *buffer);

#endif