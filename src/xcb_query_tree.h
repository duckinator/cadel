#ifndef CADEL__XCB_QUERY_TREE
#define CADEL__XCB_QUERY_TREE

#include <xcb/xcb.h>

typedef struct cadel_xcb_window_list_s {
    xcb_window_t *windows;
    size_t length;
} cadel_xcb_window_list_t;

cadel_xcb_window_list_t *cadel_xcb_query_tree(xcb_connection_t *connection,
        xcb_window_t root);

#endif
