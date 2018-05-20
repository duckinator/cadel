#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

// Subset of the arguments that get passed to xcb_create_window().
// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/
xcb_window_t cadel_xcb_create_window(xcb_connection_t *connection,
        xcb_screen_t *screen, int16_t x, int16_t y,
        uint16_t width, uint16_t height)
{
    // Request a new window ID.
    xcb_window_t window = xcb_generate_id (connection);

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

void cadel_xcb_show_window(xcb_connection_t *connection, xcb_window_t window)
{
    // Map the window to the screen.
    xcb_map_window(connection, window);

    // Wait for commands to be sent.
    xcb_flush(connection);
}

bool cadel_xcb_reparent_window(xcb_connection_t *connection,
        xcb_window_t new_parent, xcb_window_t window,
        int16_t x, int16_t y)
{
    // The return result of this tells us if it was successfully reparented.
    xcb_void_cookie_t cookie =
        xcb_reparent_window_checked(connection, window, new_parent, x, y);

    xcb_generic_error_t *error = xcb_request_check(connection, cookie);
    bool succeeded = ((error == NULL) ? true : false);

    free(error);

    return succeeded;
}

bool cadel_xcb_reparent_windows(xcb_connection_t *connection,
        xcb_screen_t *screen, xcb_window_t new_parent)
{
    size_t matches = 0;
    size_t reparented = 0;
    bool _reparented = false;

    xcb_get_property_cookie_t name_cookie;
    xcb_get_property_reply_t *name_reply;

    xcb_query_tree_cookie_t tree_cookie;
    xcb_query_tree_reply_t *tree_reply;

    tree_cookie = xcb_query_tree(connection, screen->root);
    tree_reply = xcb_query_tree_reply(connection, tree_cookie, NULL);
    if (!tree_reply) {
        return false;
    }

    xcb_window_t *children = xcb_query_tree_children(tree_reply);
    int children_length = xcb_query_tree_children_length(tree_reply);
    for (int i = 0; i < children_length; i++) {
        name_cookie = xcb_get_property(connection, 0, children[i],
                XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 100);
        name_reply = xcb_get_property_reply(connection, name_cookie, NULL);

        char *name = (char*)xcb_get_property_value(name_reply);
        if (strncmp(name, "openscad", 8) == 0) {
            printf("child window 0x%08x = %s\n", children[i], name);

            _reparented = cadel_xcb_reparent_window(connection, new_parent,
                    children[i], 0, 0);
            matches += 1;
            if (_reparented) {
                printf("SUCCEEDED reparenting 0x%08x (%s).\n", children[i], name);
                reparented += 1;
            } else {
                printf("FAILED    reparenting 0x%08x (%s).\n", children[i], name);
            }
        }

        free(name_reply);
    }

    free(tree_reply);

    if (matches == 0 || reparented == 0) {
        return false;
    }

    return true;
}
