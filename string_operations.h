#include <xcb/xcb_keysyms.h>

char** add_character_to_string(char** p_string, char character_to_add);
void erase_last_character_in_a_string(char** p_string);
char shift_character(xcb_keysym_t keysym);
