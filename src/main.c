#include "cadel.h"
#include "debug.h"
#include <err.h>
#include <errno.h>
#include "openscad.h"
#include "signal_handlers.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include "xcb_errors.h"
#include "xcb_screen.h"
#include "xcb_window.h"

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
    xcb_window_t window = cadel_xcb_create_window(connection, screen, 0, 0, 150, 150);
    // Actually display the main window.
    cadel_xcb_show_window(connection, window);

    bool openscad_start_success = cadel_openscad_start();
    if (!openscad_start_success) {
        perror(argv[0]);
        cadel_cleanup(connection);
        return errno;
    }

    // Wait until we get a Ctrl-C.
    pause();

    // If we get here, perform a clean exit.
    cadel_cleanup(connection);
    return 0;
}
