#include <stdlib.h>
#include <xcb/xcb.h>
#include "xcb_query_tree.h"

cadel_xcb_window_list_t *cadel_xcb_query_tree(xcb_connection_t *connection,
        xcb_window_t root)
{
    cadel_xcb_window_list_t *results =
        calloc(sizeof(cadel_xcb_window_list_t), 1);

    xcb_query_tree_cookie_t cookie = xcb_query_tree(connection, root);
    xcb_query_tree_reply_t *reply = xcb_query_tree_reply(connection, cookie, NULL);
    if (!reply) {
        return NULL;
    }

    results->windows = xcb_query_tree_children(reply);
    results->length  = xcb_query_tree_children_length(reply);
    results->reply   = reply;

    // TODO: Make a copy of results->windows and return that, and free `reply`.
    //free(reply);

    return results;
}
