#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include "xcb_property.h"

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

bool cadel_xcb_reparent_window(xcb_connection_t *connection,
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

bool cadel_xcb_reparent_windows(xcb_connection_t *connection,
        xcb_window_t root, xcb_window_t new_parent)
{
    char *command = NULL;
    char *class = NULL;

    xcb_query_tree_cookie_t cookie;
    xcb_query_tree_reply_t *reply = NULL;
    xcb_window_t *children = NULL;
    int children_length;

    cookie = xcb_query_tree(connection, root);
    reply = xcb_query_tree_reply(connection, cookie, NULL);
    if (!reply) {
        return false;
    }

    children = xcb_query_tree_children(reply);
    children_length = xcb_query_tree_children_length(reply);

    for (int i = 0; i < children_length; i++) {
        command =
            cadel_xcb_get_property_string(connection, children[i], "WM_COMMAND");
        class =
            cadel_xcb_get_property_string(connection, children[i], "WM_CLASS");

        printf("command='%s', class='%s'\n", command, class);

        if (strncmp(command, "openscad", 8) == 0) {
            printf("Reparenting window 0x%08x (command='%s', class='%s')\n", children[i], command, class);

            if (!cadel_xcb_hide_window(connection, children[i])) {
                warn("xcb: window 0x%08x could not be hidden.", children[i]);
            }
            if (!cadel_xcb_reparent_window(connection, new_parent,
                    children[i], 0, 0)) {
                warn("xcb: window 0x%08x could not be reparented.", children[i]);
            }

            if (!cadel_xcb_show_window(connection, children[i])) {
                warn("xcb: window 0x%08x could not be shown.", children[i]);
            }
        }

        free(command);

        cadel_xcb_reparent_windows(connection, children[i], new_parent);
    }

    free(reply);

    if (children_length == 0) {
        return false;
    }

    return true;
}
