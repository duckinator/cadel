#ifndef CADEL__XCB_PROPERTY_H
#define CADEL__XCB_PROPERTY_H

#include <xcb/xcb.h>

#define CADEL_XCB_PROPERTY_BYTES 100
// xcb uses multiples of 4 bytes, for some reason.
#define CADEL_XCB_PROPERTY_LENGTH (CADEL_XCB_PROPERTY_BYTES / 4)

char *cadel_xcb_get_property_string(char *buffer,
        xcb_connection_t *connection, xcb_window_t window, char *type);

#endif
