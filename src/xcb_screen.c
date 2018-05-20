#include <xcb/xcb.h>

xcb_screen_t *cadel_xcb_get_screen(xcb_connection_t *connection,
        int screen_number)
{
    const xcb_setup_t *setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

    // Get screen at the index +screen_number+.
    for (int i = 0; i < screen_number; i++) {
        xcb_screen_next(&iter);
    }

    return iter.data;
}
