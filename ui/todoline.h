#include <cassette/cgui.h>
#include <cassette/cobj.h>

enum TODO_THING_STATUS {
    NOT_DONE = 0, // ND
    DONE = 1, // D
};

typedef struct {
    char* name;
    enum TODO_THING_STATUS status;

    cgui_cell* cell_title;
    cgui_cell* cell_button;
} todo_thing;

static todo_thing** things = 0;
static size_t things_size = 0;

todo_thing** todo_thing_array_append(todo_thing** array_instance, size_t array_size, todo_thing* appending_instance);
todo_thing** todo_thing_array_pull(todo_thing** array_instance, size_t array_size, todo_thing* pulling_instance);
todo_thing*  todo_thing_init(char* name, void (*callback)(cgui_cell*));
void         todo_thing_status_up(todo_thing* instance);
