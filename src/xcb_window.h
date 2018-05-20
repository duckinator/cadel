#ifndef CADEL__XCB_WINDOW_H
#define CADEL__XCB_WINDOW_H

#include <stdbool.h>
#include <stdint.h>
#include <xcb/xcb.h>

xcb_window_t cadel_xcb_create_window(xcb_connection_t *connection,
        xcb_screen_t *screen, int16_t x, int16_t y,
        uint16_t width, uint16_t height);

bool cadel_xcb_show_window(xcb_connection_t *connection, xcb_window_t window);

bool cadel_xcb_reparent_windows(xcb_connection_t *connection,
        xcb_screen_t *screen, xcb_window_t new_parent_window);

#endif
