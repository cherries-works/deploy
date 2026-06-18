#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

pid_t rollback(struct Deploy *d, char *hash);
pid_t start(struct Deploy d, struct Status *s);
pid_t restart(struct Deploy d, struct Status *s);
pid_t run(struct Deploy d);

#endif
