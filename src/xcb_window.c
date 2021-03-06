#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include "xcb_property.h"
#include "xcb_query_tree.h"
#include "xcb_window.h"

// Subset of the arguments that get passed to xcb_create_window().
// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/
xcb_window_t cadel_xcb_create_window(xcb_connection_t *connection,
        xcb_screen_t *screen, int16_t x, int16_t y,
        uint16_t width, uint16_t height)
{
    // Request a new window ID.
    xcb_window_t window = xcb_generate_id(connection);

    xcb_window_t parent_window = screen->root;

    // TODO: Figure out wtf a "class" is in this context.
    uint16_t window_class = XCB_WINDOW_CLASS_INPUT_OUTPUT;
    // TODO: Figure out how this works.
    uint16_t border_width = 10;

    // Create the window.
    xcb_create_window(connection,
        XCB_COPY_FROM_PARENT,   // Copy depth from parent window.
        window,
        parent_window,
        x, y,
        width, height,
        border_width,
        window_class,
        screen->root_visual,    // What is "visual"?
        0, NULL);               // XCB docs say don't use these.

    return window;
}

bool cadel_xcb_show_window(xcb_connection_t *connection, xcb_window_t window)
{
    // Map the window to the screen.
    xcb_void_cookie_t cookie = xcb_map_window_checked(connection, window);

    // Wait for commands to be sent.
    xcb_flush(connection);

    xcb_generic_error_t *error = xcb_request_check(connection, cookie);
    bool succeeded = ((error == NULL) ? true : false);

    return succeeded;
}

bool cadel_xcb_hide_window(xcb_connection_t *connection, xcb_window_t window)
{
    // Unmap the window from the screen.
    xcb_void_cookie_t cookie = xcb_unmap_window_checked(connection, window);

    // Wait for commands to be sent.
    xcb_flush(connection);

    xcb_generic_error_t *error = xcb_request_check(connection, cookie);
    bool succeeded = ((error == NULL) ? true : false);

    return succeeded;
}

bool _cadel_xcb_reparent_window(xcb_connection_t *connection,
        xcb_window_t new_parent, xcb_window_t window,
        int16_t x, int16_t y)
{
    // The return result of this tells us if it was successfully reparented.
    xcb_void_cookie_t cookie =
        xcb_reparent_window_checked(connection, window, new_parent, x, y);

    xcb_flush(connection);

    xcb_generic_error_t *error = xcb_request_check(connection, cookie);
    bool succeeded = ((error == NULL) ? true : false);

    free(error);

    return succeeded;
}

bool cadel_xcb_reparent_window(xcb_connection_t *connection,
        xcb_window_t new_parent, xcb_window_t window,
        int16_t x, int16_t y)
{
    if (!cadel_xcb_hide_window(connection, window)) {
        warn("xcb: window 0x%08x could not be hidden.", window);
        return false;
    }
    if (!_cadel_xcb_reparent_window(connection, new_parent,
            window, x, y)) {
        warn("xcb: window 0x%08x could not be reparented.", window);
        return false;
    }

    if (!cadel_xcb_show_window(connection, window)) {
        warn("xcb: window 0x%08x could not be shown.", window);
        return false;
    }

    return true;
}

bool cadel_xcb_reparent_callback(xcb_connection_t *connection,
        xcb_window_t root, xcb_window_t new_parent, xcb_window_t child)
{
    char command[CADEL_XCB_PROPERTY_BYTES] = {0,};

    cadel_xcb_get_property_string((char*)&command, connection, child, "WM_COMMAND");

    cadel_xcb_reparent_windows(connection, child, new_parent);

    if (strncmp(command, "cadel-", 6) == 0) {
        printf("Reparenting window 0x%08x (command='%s')\n", child, command);
        cadel_xcb_reparent_window(connection, new_parent, child, 0, 0);
    }
}

bool cadel_xcb_reparent_windows(xcb_connection_t *connection,
        xcb_window_t root, xcb_window_t new_parent)
{
    cadel_xcb_window_list_t children = {{0,}, 0};
    if (!cadel_xcb_query_tree(&children, connection, root)) {
        return false;
    }

    for (size_t i = 0; i < children.length; i++) {
        cadel_xcb_reparent_callback(connection, root, new_parent, children.windows[i]);
    }

    return true;
}
