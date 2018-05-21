#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "xcb_property.h"
#include "xcb_window.h"

bool cadel_openscad_reparent(xcb_connection_t *connection,
        xcb_window_t root, xcb_window_t new_parent, xcb_window_t child)
{
    char command[CADEL_XCB_PROPERTY_BYTES] = {0,};

    cadel_xcb_get_property_string((char*)&command, connection, child, "WM_COMMAND");

    cadel_xcb_reparent_windows(connection, child, new_parent, cadel_openscad_reparent);

    if (strncmp(command, "openscad", 8) == 0) {
        printf("Reparenting window 0x%08x (command='%s')\n", child, command);
        cadel_xcb_reparent_window(connection, new_parent, child, 0, 0);
    }

    return true;
}


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
    } else {
        // HACK: Wait for OpenSCAD to start.
        sleep(1);
    }

    return pid;
}
