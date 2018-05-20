#include <stdlib.h>
#include <xcb/xcb.h>
#include "xcb_query_tree.h"

codel_xcb_window_list_t *codel_xcb_query_tree(xcb_connection_t *connection,
        xcb_window_t root)
{
    codel_xcb_window_list_t *results =
        calloc(sizeof(codel_xcb_window_list_t), 1);

    xcb_query_tree_cookie_t cookie = xcb_query_tree(connection, root);
    xcb_query_tree_reply_t *reply = xcb_query_tree_reply(connection, cookie, NULL);
    if (!reply) {
        return NULL;
    }

    results->windows = xcb_query_tree_children(reply);
    results->length  = xcb_query_tree_children_length(reply);

    free(reply);

    return results;
}
