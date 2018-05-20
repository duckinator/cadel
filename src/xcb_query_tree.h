#ifndef CADEL__XCB_QUERY_TREE
#define CADEL__XCB_QUERY_TREE

#include <stdbool.h>
#include <xcb/xcb.h>

typedef struct cadel_xcb_window_list_s {
    // If you have over 2k windows, you probably have bigger problems
    // than this not working.
    xcb_window_t windows[2048];
    size_t length;
} cadel_xcb_window_list_t;

bool cadel_xcb_query_tree(cadel_xcb_window_list_t *results,
        xcb_connection_t *connection, xcb_window_t root);

#endif
