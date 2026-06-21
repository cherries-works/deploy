#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

pid_t rollback(Deploy *d, char *hash);
pid_t start(Deploy d, Status *s);
pid_t restart(Deploy d, Status *s);
pid_t run(Deploy d);

#endif
