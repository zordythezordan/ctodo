#include <cassette/cgui-button.h>
#include <cassette/cgui-cell.h>
#include <cassette/cgui-label.h>
#include <string.h>

#include "todoline.h"

const char* TODO_THING_STATUS_NAMES[] = {
    "ND",
    "D"
};
const size_t TODO_THING_STATUS_NAMES_LENGTH = 2;

todo_thing** todo_thing_array_append(todo_thing** array_instance, size_t array_size, todo_thing* appending_instance){
    array_instance = realloc(array_instance, (array_size + 1) * sizeof(todo_thing));
    array_instance[array_size] = malloc(sizeof(todo_thing));
    memcpy(array_instance[array_size], appending_instance, sizeof(todo_thing));
    return array_instance;
}

todo_thing** todo_thing_array_pull(todo_thing** array_instance, size_t array_size, todo_thing* pulling_instance){
    int index = -1;
    for (int i = 0; i < array_size; i++){
        if (array_instance[i] == pulling_instance){
            index = i;
            break;
        }
    }

    if (index == -1)
        return array_instance;

    todo_thing** new_array = malloc((array_size - 1) * sizeof(todo_thing*));
    int offset = 0;
    for (int i = 0; i < array_size; i++){
        if (i == index){
            offset = 1;
            continue;
        }

        new_array[i - offset] = malloc(sizeof(todo_thing));
        memcpy(new_array[i - offset], array_instance[i], sizeof(todo_thing));
    }

    return new_array;
}

todo_thing* todo_thing_init(char* name, void (*callback)(cgui_cell*)){
    if (name == NULL || callback == NULL){
        return NULL;
    }
    
    todo_thing* thing = malloc(sizeof(todo_thing));
    if (thing == NULL){
        return NULL;
    }
    
    thing->status = NOT_DONE;

    thing->name = strdup(name);
    if (thing->name == NULL){
        free(thing);
        return NULL;
    }

    thing->cell_title = cgui_label_create();
    if (thing->cell_title == CGUI_CELL_PLACEHOLDER){
        free(thing->name);
        cgui_cell_destroy(thing->cell_title);
        free(thing);
        return NULL;
    }
    cgui_label_set(thing->cell_title, thing->name);
    
    thing->cell_button = cgui_button_create();
    if (thing->cell_button == CGUI_CELL_PLACEHOLDER){
        free(thing->name);
        cgui_cell_destroy(thing->cell_title);
        cgui_cell_destroy(thing->cell_button);
        free(thing);
        return NULL;
    }
    
    cgui_button_set_label(thing->cell_button, TODO_THING_STATUS_NAMES[thing->status]);
    cgui_button_on_click_no_arg(thing->cell_button, callback);
    
    return thing;
}

void todo_thing_status_up(todo_thing* instance){ // idk if ill ever need status-- stuff
    if (instance->status + 1 >= TODO_THING_STATUS_NAMES_LENGTH)
        instance->status = 0;
    else
        instance->status++;

    cgui_button_set_label(instance->cell_button, TODO_THING_STATUS_NAMES[instance->status]);
}
    
