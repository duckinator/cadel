#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include "xcb_query_tree.h"

#include <stdio.h>

cadel_xcb_window_list_t *cadel_xcb_query_tree(xcb_connection_t *connection,
        xcb_window_t root)
{
    cadel_xcb_window_list_t *results =
        calloc(sizeof(cadel_xcb_window_list_t), 1);
    xcb_window_t *windows = NULL;

    xcb_query_tree_cookie_t cookie = xcb_query_tree(connection, root);
    xcb_query_tree_reply_t *reply = xcb_query_tree_reply(connection, cookie, NULL);
    if (!reply) {
        return NULL;
    }

    windows = xcb_query_tree_children(reply);
    results->length  = xcb_query_tree_children_length(reply);

    results->windows = calloc(results->length, sizeof(xcb_window_t));
    memcpy(results->windows, windows, sizeof(xcb_window_t) * results->length);

    free(reply);

    return results;
}
