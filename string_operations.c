#include <stdlib.h>
#include <stdio.h>
#include <X11/keysym.h>
#include <xcb/xcb_keysyms.h>
#include <ctype.h>
#include <string.h>

void add_character_to_string(char** p_string, char character_to_add){
    size_t length = (*p_string == NULL) ? 0 : strlen(*p_string);

    if (length == 0){
        *p_string = malloc(2 * sizeof(char));

        (*p_string)[0] = character_to_add;
        (*p_string)[1] = '\0';
    }
    else {
        char* copy = strdup(*p_string);
        char* resized = realloc(copy, (length + 2) * sizeof(char));
        if (resized == NULL) 
            return;

        *p_string = resized;
        (*p_string)[length] = character_to_add;
        (*p_string)[length + 1] = '\0';
    }
}

void erase_last_character_in_a_string(char** p_string){
    if (p_string == NULL || *p_string == NULL)
        return;
    
    size_t length = strlen(*p_string);

    if (length > 1){
        char* copy = strdup(*p_string);
        char* resized = realloc(copy, (length - 1) * sizeof(char));

        if (resized == NULL)
            return;

        *p_string = resized;
        (*p_string)[length - 1] = '\0';
    }
    else {
        *p_string = malloc(1);
        *p_string[0] = '\0';
    }
}

char shift_character(xcb_keysym_t keysym){
    char character = (char)keysym;
    
    switch (keysym) {
        case XK_1: character = '!'; break;
        case XK_2: character = '@'; break;
        case XK_3: character = '#'; break;
        case XK_4: character = '$'; break;
        case XK_5: character = '%'; break;
        case XK_6: character = '^'; break;
        case XK_7: character = '&'; break;
        case XK_8: character = '*'; break;
        case XK_9: character = '('; break;
        case XK_0: character = ')'; break;
        case XK_backslash: character = '|'; break;
        case XK_slash: character = '?'; break;
        case XK_period: character = '>'; break;
        case XK_comma: character = '<'; break;
        case XK_minus: character = '_'; break;
        case XK_equal: character = '+'; break;
        case XK_apostrophe: character = '"'; break;
        case XK_semicolon: character = ':'; break;
        case XK_bracketleft: character = '{'; break;
        case XK_bracketright: character = '}'; break;
        case XK_grave: character = '~'; break;
        default: character = toupper((char)keysym); break;
    }

    return character;
}

