#ifndef CADEL__XCB_SCREEN_H
#define CADEL__XCB_SCREEN_H

#include <xcb/xcb.h>

xcb_screen_t *cadel_xcb_get_screen(xcb_connection_t *connection,
        int screen_number);

#endif
