#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

char *cadel_xcb_get_property_string(xcb_connection_t *connection,
        xcb_window_t window, xcb_atom_t type)
{
    xcb_get_property_cookie_t cookie;
    xcb_get_property_reply_t *reply;
    char *original_value;
    char *value;

    cookie = xcb_get_property(connection, 0, window, type,
            XCB_ATOM_STRING, 0, 100);
    reply = xcb_get_property_reply(connection, cookie, NULL);

    original_value = (char*)xcb_get_property_value(reply);

    value = malloc(strlen(original_value));
    strcpy(value, original_value);

    free(reply);

    return value;
}
