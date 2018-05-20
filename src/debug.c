#include <stdio.h>
#include <xcb/xcb.h>

void cadel_print_screen_information(xcb_screen_t *screen)
{
    printf("\n");
    printf("Informations of screen %u:\n", screen->root);
    printf("  width.........: %u\n", screen->width_in_pixels);
    printf("  height........: %u\n", screen->height_in_pixels);
    printf("  white pixel...: %u\n", screen->white_pixel);
    printf("  black pixel...: %u\n", screen->black_pixel);
    printf("\n");
}
