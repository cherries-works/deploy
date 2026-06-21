#ifndef SETUP_H
#define SETUP_H

#include "config.h"

extern void setup();
extern void setupDeploy(Deploy *d);
extern void setupPath(Deploy d, char *buffer);
extern void setupPathHash(Deploy d, char *hash, char *buffer);

#endif