#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

pid_t cadel_openscad_start()
{
    char *argv[] = {
        "openscad",
        NULL,
    };

    pid_t pid = fork();

    if (pid == -1) {
        return -1;
    }

    if (pid == 0) {
        int result = execv("/usr/bin/openscad", argv);
        if (result == -1) {
            return -1;
        }
        // Child process does not get past here.
    }

    return pid;
}
