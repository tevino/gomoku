#ifndef POPEN2_H

#define POPEN2_H

#include <unistd.h>

pid_t popen2 (int *tty, char *cmd[]);

#endif
