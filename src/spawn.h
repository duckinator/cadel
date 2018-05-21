#ifndef CADEL__SPAWN_H
#define CADEL__SPAWN_H

#include <unistd.h>

pid_t cadel_spawn(char *filename, char *new_argv0, char *argv_[]);

#endif
