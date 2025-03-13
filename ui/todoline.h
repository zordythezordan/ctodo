#include <cassette/cgui.h>
#include <cassette/cobj.h>

enum TODO_THING_STATUS {
    NOT_DONE = 0, // ND
    IN_PROGRESS = 1, // INP
    DONE = 2, // D
};

extern const char* TODO_THING_STATUS_NAMES[];
extern const size_t TODO_THING_STATUS_NAMES_LENGTH;

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
todo_thing*  todo_thing_init(char* name);
void         todo_thing_status_up(todo_thing* instance);
