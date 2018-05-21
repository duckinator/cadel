#include "spawn.h"
#include <unistd.h>

pid_t cadel_spawn(char *filename, char *argv[])
{
    pid_t pid = fork();

    if (pid == 0) {     // Child process only.
        execv(filename, argv);
        return -1;      // Child process should not get here.
    }

    return pid;
}
