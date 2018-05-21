#include "cadel.h"
#include "debug.h"
#include <err.h>
#include <errno.h>
#include "openscad.h"
#include "signal_handlers.h"
#include "spawn.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include "xcb_errors.h"
#include "xcb_screen.h"
#include "xcb_window.h"

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)
#define PRINT_ERROR() perror(__FILE__ ":" STRINGIFY(__LINE__));

void cadel_cleanup(xcb_connection_t *connection)
{
    xcb_disconnect(connection);
}

int main(int argc, char *argv[])
{
    int screen_number;
    xcb_connection_t *connection = NULL;
    xcb_screen_t *screen = NULL;

    bool handlers_registered = cadel_register_signal_handlers();
    if (!handlers_registered) {
        warn("Could not set signal handlers.");
        return 1;
    }

    connection = xcb_connect(NULL, &screen_number);
    int connection_error_code = xcb_connection_has_error(connection);
    if (connection_error_code != 0) {
        cadel_xcb_print_error(connection_error_code);
        cadel_cleanup(connection);
        return connection_error_code;
    }

    screen = cadel_xcb_get_screen(connection, screen_number);
    if (screen == NULL) {
        warn("xcb: Could not get screen %i.", screen_number);
        cadel_cleanup(connection);
        return 1;
    }
    cadel_print_screen_information(screen);

    // Create the main window.
    xcb_window_t window = cadel_xcb_create_window(connection, screen, 0, 0, 500, 500);
    // Actually display the main window.
    if (!cadel_xcb_show_window(connection, window)) {
        PRINT_ERROR();
        cadel_cleanup(connection);
        return errno;
    }

    pid_t openscad_pid = cadel_spawn("/usr/bin/openscad", "cadel-openscad", argv);
    if (openscad_pid == -1) {
        PRINT_ERROR();
        cadel_cleanup(connection);
        return errno;
    }
    // HACK: Wait for OpenSCAD to start.
    sleep(1);


    if (!cadel_xcb_reparent_windows(connection, screen->root, window,
                cadel_openscad_reparent)) {
        PRINT_ERROR();
        cadel_cleanup(connection);
        return errno;
    }

    // Wait until we get a Ctrl-C.
    pause();

    // If we get here, perform a clean exit.
    cadel_cleanup(connection);
    return 0;
}
