#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

bool cadel_openscad_start()
{
    char *argv[] = {
        "openscad",
        NULL,
    };

    pid_t pid = fork();

    if (pid == -1) {
        return false;
    }

    if (pid == 0) {
        int result = execv("/usr/bin/openscad", argv);
        if (result == -1) {
            return false;
        }
    } else {
        printf("Hello from parent.\n");
        int status;
        (void)waitpid(pid, &status, 0);
    }

    return true;
}
