#include "color.h"

#include <string.h>


const char *
color_from_name(const char *color_name)
{
    if (strcmp(color_name, "black") == 0)
        return COLOR_BLACK;

    if (strcmp(color_name, "red") == 0)
        return COLOR_RED;

    if (strcmp(color_name, "green") == 0)
        return COLOR_GREEN;

    if (strcmp(color_name, "yellow") == 0)
        return COLOR_YELLOW;

    if (strcmp(color_name, "blue") == 0)
        return COLOR_BLUE;

    if (strcmp(color_name, "purple") == 0)
        return COLOR_PURPLE;

    if (strcmp(color_name, "cyan") == 0)
        return COLOR_CYAN;

    if (strcmp(color_name, "white") == 0)
        return COLOR_WHITE;

    if (strcmp(color_name, "orange") == 0)
        return COLOR_CYAN;

    return NULL;
}