#include <cassette/cgui-button.h>
#include <cassette/cgui-cell.h>
#include <cassette/cgui-event.h>
#include <cassette/cgui-grid.h>
#include <cassette/cgui-window.h>
#include <cassette/cgui.h>
#include "ui/todoline.h"
#include <X11/keysym.h>
#include <xcb/xcb_keysyms.h>

cgui_grid* update_todo_things();

void input_callback(struct cgui_event* event);
void status_button_callback(cgui_cell* c);
void add_button_callback(cgui_cell* c);

cgui_window* window = CGUI_WINDOW_PLACEHOLDER;
cgui_grid* grid = CGUI_GRID_PLACEHOLDER;
cgui_cell* add_button = CGUI_CELL_PLACEHOLDER;

bool is_shift_pressed = false;

int main(int argc, char** argv){
    cgui_init(argc, argv);

	window = cgui_window_create();
	grid = cgui_grid_create(2, 7);
	add_button = cgui_button_create();
	cgui_button_set_label(add_button, "+");
	cgui_button_on_click_no_arg(add_button, add_button_callback);

    cgui_event_on_event(input_callback);

	char* to_init[] = {
		"Finish TODO Program",
		"Implement proper ui",
		"Implement deleting things",
	};
	
	size_t size = sizeof(to_init)/sizeof(to_init[0]);

	for (int i = 0; i < size; i++){
	    todo_thing* thing = todo_thing_init(to_init[i], status_button_callback);

	    if (thing == NULL)
	    	continue;

		cgui_grid_assign_cell(grid, thing->cell_title, 0, i, 1, 1);
		cgui_grid_assign_cell(grid, thing->cell_button, 1, i, 1, 1);
			
		things = todo_thing_array_append(things, things_size, thing);
		things_size++;
	}
	
	cgui_grid_assign_cell(grid, add_button, 0, 6, 2, 1);

	cgui_grid_resize_col(grid, 0, 50);
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
	cgui_grid* new_grid = cgui_grid_create(2, 7);

	for (int i = 0; i < things_size; i++){
		cgui_grid_assign_cell(new_grid, things[i]->cell_title, 0, i, 1, 1);
		cgui_grid_assign_cell(new_grid, things[i]->cell_button, 1, i, 1, 1);
	}

	if (things_size < 7){
		cgui_grid_assign_cell(new_grid, add_button, 0, 6, 2, 1);
	}
	
	cgui_grid_resize_col(new_grid, 0, 50);
	cgui_grid_set_col_flex(new_grid, 0, 2);

	cgui_window_swap_grid(window, grid, new_grid);
	cgui_window_activate(window);
	
	return new_grid;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc23-extensions"
void input_callback(struct cgui_event* event){
	switch(event->type){
		case CGUI_EVENT_KEY_PRESS:
            if (event->key_code == 50 || event->key_code == 62)
            	is_shift_pressed = true;
			break;
		case CGUI_EVENT_KEY_RELEASE:
            if (event->key_code == 50 || event->key_code == 62)
            	is_shift_pressed = false;

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
	char name[24];
	sprintf(name, "Implement typing!! %lu", things_size + 1);

	todo_thing* new_thing = todo_thing_init(name, status_button_callback);
	things = todo_thing_array_append(things, things_size, new_thing);
	things_size++;
	grid = update_todo_things();
}
