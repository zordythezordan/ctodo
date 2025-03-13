#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>
#include <linux/limits.h>
#include <sys/stat.h>

#include "saving.h"
#include "string_operations.h"
#include "limits.h"

char* get_pwd(){
    char* cwd = malloc(PATH_MAX * sizeof(char));
    
    if (getcwd(cwd, sizeof(PATH_MAX)) != NULL)
        return cwd;
    
    return NULL;
}

int io_object_exists(const char *path) {
    return access(path, F_OK) == 0;
}

char* todo_to_string(const todo_thing* thing){
    char* name = strdup(thing->name);
    char* status = strdup(TODO_THING_STATUS_NAMES[thing->status]);

    char* final_string = strconcat(strconcat(name, strconcat("\t", status)), "\n");

    return final_string;
}

                        // [ "name", "status" ]
todo_thing* string_to_todo(char** split_result){
    todo_thing* thing = todo_thing_init(split_result[0]);

    int status_index = 0;
    for (int i = 0; i < (int)TODO_THING_STATUS_NAMES_LENGTH; i++){
        if (strcmp(TODO_THING_STATUS_NAMES[i], split_result[1]) == 0){
            status_index = i;
            break;
        }
    }

    thing->status = status_index;
    cgui_button_set_label(thing->cell_button, TODO_THING_STATUS_NAMES[thing->status]);

    return thing;
}

void write_todo_to_file(todo_thing** things, size_t things_size){
    char* full_path = strconcat(getenv("HOME"), "/.local/share/ctodo/");

    if (io_object_exists(full_path) == 0){
        int status = mkdir(full_path, 0755);
        if (status != 0){
            fprintf(stderr, "rips your skin off cutely\n");
            perror("mkdir");
            return;
        }
    }

    char* file_path = strconcat(full_path, "todo_data");
    FILE* file = fopen(file_path, "w");

    for (int i = 0; i < (int)things_size; i++){
        char* to_string = todo_to_string(things[i]);
        fputs(to_string, file);
    }

    fclose(file);
}

todo_thing** load_from_file(size_t* size){
    FILE* file = fopen(strconcat(getenv("HOME"), "/.local/share/ctodo/todo_data"), "r");
    if (file == NULL){
        printf("[ERROR] Failed to load todo_data, not critical.\n");
        return NULL;
    }

    char buffer[256];
    todo_thing** things = NULL;
    size_t things_size = 0;

    while(fgets(buffer, sizeof(buffer), file) != NULL && things_size < ROWS_COUNT){
        buffer[strcspn(buffer, "\n")] = '\0';

        char** split = strsplit(buffer, '\t');
        
        todo_thing* thing = string_to_todo(split);
        things = todo_thing_array_append(things, things_size, thing);
        things_size++;
    }

    fclose(file);

    *size = things_size;
    return things;
}
