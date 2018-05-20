#ifndef CADEL__XCB_PROPERTY_H
#define CADEL__XCB_PROPERTY_H

#include <xcb/xcb.h>

char *cadel_xcb_get_property_string(xcb_connection_t *connection,
        xcb_window_t window, xcb_atom_t type);

#endif
