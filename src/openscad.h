#ifndef CADEL__OPENSCAD_H
#define CADEL__OPENSCAD_H

#include <stdbool.h>
#include <xcb/xcb.h>

bool cadel_openscad_reparent(xcb_connection_t *connection,
        xcb_window_t root, xcb_window_t new_parent, xcb_window_t child);

bool cadel_openscad_start();

#endif
