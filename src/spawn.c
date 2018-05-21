#include "spawn.h"
#include <string.h>
#include <unistd.h>

// A thin wrapper around execv(), with the ability to override argv[0].
pid_t cadel_spawn(char *filename, char *new_argv0, char *argv_[])
{
    pid_t pid;
    char **argv;

    // Make +argv+ the same as +argv_+, but change argv[0] to +new_argv0+.
    memcpy(&argv, &argv_, sizeof(*argv_));
    if (new_argv0 != NULL) {
        argv[0] = new_argv0;
    }

    pid = fork();

    if (pid == 0) {     // Child process only.
        execv(filename, argv);
        return -1;      // Child process should not get here.
    }

    return pid;
}
