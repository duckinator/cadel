#include <err.h>
#include <xcb/xcb.h>
#include "xcb_errors.h"

void cadel_xcb_print_error(int error)
{
    switch(error) {
        case XCB_CONN_ERROR:
            warn("xcb: failed to connect to default display.");
            break;
        case XCB_CONN_CLOSED_EXT_NOTSUPPORTED:
            warn("xcb: attempted to use unsupported extension.");
            break;
        case XCB_CONN_CLOSED_MEM_INSUFFICIENT:
            warn("xcb: insufficient memory.");
            break;
        case XCB_CONN_CLOSED_REQ_LEN_EXCEED:
            warn("xcb: message length exceeded.");
            break;
        case XCB_CONN_CLOSED_PARSE_ERR:
            warn("xcb: could not parse display string.");
            break;
        case XCB_CONN_CLOSED_INVALID_SCREEN:
            warn("xcb: no such screen.");
            break;
    }
}

