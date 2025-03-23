#include "ui/todoline.h"

char*        todo_to_string(const todo_thing* thing);
todo_thing*  string_to_todo(char** split_result);
void         write_todo_to_file(todo_thing** things, size_t things_size);
todo_thing** load_from_file(size_t* size);
