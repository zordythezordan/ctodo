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

char** append_string(char** array, size_t array_size, char* string){
    char** new_array = realloc(array, (array_size + 1) * sizeof(char*));
    new_array[array_size] = strdup(string);
    return new_array;
}

char** strsplit(char* str, char split_char) {
    char* split_pos = strchr(str, split_char);
    if (split_pos == NULL) {
        return NULL;
    }

    char** result = malloc(2 * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    size_t len1 = split_pos - str;

    result[0] = malloc(len1 + 1);
    if (result[0] == NULL) {
        free(result);
        return NULL;
    }
    strncpy(result[0], str, len1);
    result[0][len1] = '\0';

    size_t len2 = strlen(split_pos + 1);

    result[1] = malloc(len2 + 1);
    if (result[1] == NULL) {
        free(result[0]); 
        free(result);
        return NULL;
    }
    strcpy(result[1], split_pos + 1);

    return result;
}

char* strconcat(char* string1, char* string2){
    char* new_string = malloc(strlen(string1) + strlen(string2));

    strcpy(new_string, string1);
    strcat(new_string, string2);

    return new_string;
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

