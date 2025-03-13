#include <cassette/cgui-button.h>
#include <cassette/cgui-cell.h>
#include <cassette/cgui-event.h>
#include <cassette/cgui-grid.h>
#include <cassette/cgui-label.h>
#include <cassette/cgui-window.h>
#include <cassette/cgui.h>
#include <X11/keysym.h>
#include <xcb/xcb_keysyms.h>

// #include "ui/todoline.h"
#include "string_operations.h"
#include "saving.h"

#define ROWS_COUNT 7

cgui_grid* update_todo_things();

void input_callback(struct cgui_event* event);
void status_button_callback(cgui_cell* c);
void add_button_callback(cgui_cell* c);
bool label_hover_callback(cgui_cell* c, struct cgui_cell_event* event);
void window_on_exit();

cgui_window* window = CGUI_WINDOW_PLACEHOLDER;
cgui_grid* grid = CGUI_GRID_PLACEHOLDER;
cgui_cell* add_button = CGUI_CELL_PLACEHOLDER;

bool is_shift_pressed = false;

bool is_editing_instance = false;
todo_thing* editing_instance;

int main(int argc, char** argv){
    cgui_init(argc, argv);

	window = cgui_window_create();
	grid = cgui_grid_create(2, ROWS_COUNT);
	add_button = cgui_button_create();
	cgui_button_set_label(add_button, "+");
	cgui_button_on_click_no_arg(add_button, add_button_callback);

    cgui_event_on_event(input_callback);
    cgui_on_exit(window_on_exit);

	things = load_from_file(&things_size);
	for (int i = 0; i < things_size; i++){
		cgui_grid_assign_cell(grid, things[i]->cell_title, 0, i, 1, 1);
		cgui_grid_assign_cell(grid, things[i]->cell_button, 1, i, 1, 1);
		cgui_cell_on_event(things[i]->cell_title, label_hover_callback);
		cgui_button_on_click_no_arg(things[i]->cell_button, status_button_callback);
	}

	cgui_grid_assign_cell(grid, add_button, 0, ROWS_COUNT - 1, 2, 1);

	cgui_grid_resize_col(grid, 0, 45);
	cgui_grid_resize_col(grid, 1, 5);
	cgui_grid_set_col_flex(grid, 0, 2);
	
    cgui_window_push_grid(window, grid);
	cgui_window_rename(window, "meow");
	cgui_window_activate(window);

	cgui_run();

	if (cgui_error())
	{
		printf("Gui has failed during operation (%i).\n", cgui_error());
	}

	cgui_window_destroy(window);
	cgui_grid_destroy(grid);

	cgui_reset();
}

cgui_grid* update_todo_things(){
	cgui_grid* new_grid = cgui_grid_create(2, ROWS_COUNT);

	for (int i = 0; i < things_size; i++){
		cgui_grid_assign_cell(new_grid, things[i]->cell_title, 0, i, 1, 1);
		cgui_grid_assign_cell(new_grid, things[i]->cell_button, 1, i, 1, 1);
	}

	if (things_size < ROWS_COUNT){
		cgui_grid_assign_cell(new_grid, add_button, 0, ROWS_COUNT - 1, 2, 1);
	}
	
	cgui_grid_resize_col(new_grid, 0, 45);
	cgui_grid_resize_col(new_grid, 1, 5);
	cgui_grid_set_col_flex(new_grid, 0, 2);

	cgui_window_swap_grid(window, grid, new_grid);

	return new_grid;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc23-extensions"
void input_callback(struct cgui_event* event){
	switch(event->type){
		case CGUI_EVENT_KEY_PRESS:
            if (event->key_code == 50 || event->key_code == 62)
            	is_shift_pressed = true;
            if (is_editing_instance == true){
	            // typing
				if (event->key_code > 9 && event->key_code < 62 && event->key_code != 36 && event->key_code != 22){
            		xcb_key_symbols_t* keysyms = xcb_key_symbols_alloc(cgui_x11_connection());
            		if (keysyms == NULL){
            		    fprintf(stderr, "Couldn't allocate keysyms\n");
            		    return;
            		}

            		xcb_keysym_t keysym = xcb_key_symbols_get_keysym(keysyms, event->key_code, 0);

            		char character = 0;
            		if (keysym >= XK_space && keysym <= XK_asciitilde){
            		    character = event->key_mods.shift == false && event->key_mods.capslock == false ? (char)keysym : shift_character(keysym);

            		    add_character_to_string(&editing_instance->name, character);
            		    cgui_label_set(editing_instance->cell_title, editing_instance->name);
            		}
            	}
            	else if (event->key_code == 65){
            		add_character_to_string(&editing_instance->name, ' ');
            		cgui_label_set(editing_instance->cell_title, editing_instance->name);
            	}
        		else if (event->key_code == 22){
            		if (event->key_mods.ctrl == true){
            			editing_instance->name = malloc(1);
            			editing_instance->name[0] = '\0';
            		}
            		else {
						erase_last_character_in_a_string(&editing_instance->name);
            		}

            		cgui_label_set(editing_instance->cell_title, editing_instance->name);
            	}
            	else if (event->key_code == 9 || event->key_code == 36){
            		is_editing_instance = false;
            		editing_instance = NULL;
            	}
            }
			break;
		case CGUI_EVENT_KEY_RELEASE:
            if (event->key_code == 50 || event->key_code == 62)
            	is_shift_pressed = false;
            break;

		default: break;
	}
}
#pragma clang diagnostic pop

void status_button_callback(cgui_cell* c){
	for (int i = 0; i < things_size; i++){
		todo_thing* data = things[i];
		if (c == data->cell_button) {
			if (is_shift_pressed == true){
				things = todo_thing_array_pull(things, things_size, data);
				things_size--;
				grid = update_todo_things();
			}
			else {
				todo_thing_status_up(data);
			}
		}
	}
}

void add_button_callback(cgui_cell* c){
	todo_thing* new_thing = todo_thing_init("");
	cgui_button_on_click_no_arg(new_thing->cell_button, status_button_callback);
	cgui_cell_on_event(new_thing->cell_title, label_hover_callback);
	things = todo_thing_array_append(things, things_size, new_thing);
	things_size++;
	grid = update_todo_things();
}

bool label_hover_callback(cgui_cell* c, struct cgui_cell_event* event){
	todo_thing* thing = NULL;

	for (int i = 0; i < things_size; i++){
		if (c == things[i]->cell_title){
			thing = things[i];
			break;
		}
	}
	
	if (thing == NULL){
		return false;
	}
	
	switch(event->type){
		case CGUI_CELL_EVENT_BUTTON_PRESS:
			if (event->button_id == 1){
				is_editing_instance = true;
				editing_instance = thing;
			}
			break;

		default: break;
	}

	return true;
}

void window_on_exit(){
	write_todo_to_file(things, things_size);
}
