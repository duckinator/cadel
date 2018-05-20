#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

char *cadel_xcb_get_property_string(xcb_connection_t *connection,
        xcb_window_t window, char *type)
{
    xcb_intern_atom_cookie_t atom_cookie;
    xcb_intern_atom_reply_t *atom_reply;
    xcb_atom_t type_atom;

    xcb_get_property_cookie_t cookie;
    xcb_get_property_reply_t *reply;
    char *original_value;
    char *value;

    atom_cookie = xcb_intern_atom(connection, 0, strlen(type), type);
    atom_reply = xcb_intern_atom_reply(connection, atom_cookie, NULL);
    if (atom_reply == NULL) {
        return NULL;
    }

    type_atom = atom_reply->atom;
    free(atom_reply);

    cookie = xcb_get_property(connection, 0, window, type_atom,
            XCB_ATOM_STRING, 0, 100);
    reply = xcb_get_property_reply(connection, cookie, NULL);
    if (reply == NULL) {
        return NULL;
    }

    original_value = (char*)xcb_get_property_value(reply);

    value = malloc(strlen(original_value));
    strcpy(value, original_value);

    free(reply);

    return value;
}
