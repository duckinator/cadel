#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
    bool succeeded = (error ? false : true);

    free(error);

    return succeeded;
}
