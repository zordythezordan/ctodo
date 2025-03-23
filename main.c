#include <cassette/cgui-button.h>
#include <cassette/cgui-cell.h>
#include <cassette/cgui-filler.h>
#include <cassette/cgui-grid.h>
#include <cassette/cgui-label.h>
#include <cassette/cgui-stripes.h>
#include <cassette/cgui-window.h>
#include <cassette/cgui.h>
// #include <signal.h>
#include <stdbool.h>
#include <stdio.h>

#include "recording.h"

void button_record_callback(cgui_cell* c);
void button_pause_callback(cgui_cell* c);
void window_destroy_callback(cgui_window* w);

enum RECORDING_STATE {
    STOPPED = 0,
    RECORDING = 1,
    PAUSED = 2,
};

void label_set_state(enum RECORDING_STATE state);

cgui_cell* button_pause = CGUI_CELL_PLACEHOLDER;
cgui_cell* label = CGUI_CELL_PLACEHOLDER;

int pid = -1;
bool started_recording = false;
bool paused_recording  = false;

char** file_names = NULL;
size_t file_names_length = 0;

char* today = NULL;

int main(int argc, char** argv){
    get_today();
    
    cgui_init(argc, argv);

    cgui_window* window = cgui_window_create();
    cgui_grid* grid = cgui_grid_create(3, 2);

    label = cgui_label_create();
    label_set_state(STOPPED);

    cgui_cell* button_record = cgui_button_create();
    button_pause = cgui_button_create();

    cgui_button_set_label(button_record, "Record");
    cgui_button_on_click(button_record, button_record_callback);
    
    cgui_button_set_label(button_pause, "Pause");
    cgui_button_on_click(button_pause, button_pause_callback);
    cgui_button_disable(button_pause);

    cgui_cell* filler = cgui_stripes_create();
    
    cgui_grid_assign_cell(grid, label, 0, 0, 3, 1);
    cgui_grid_assign_cell(grid, filler, 0, 1, 1, 1);
    cgui_grid_assign_cell(grid, button_record, 1, 1, 1, 1);
    cgui_grid_assign_cell(grid, button_pause, 2, 1, 1, 1);

    cgui_grid_set_col_flex(grid, 0, 1);
    cgui_grid_set_col_flex(grid, 1, 1);
    cgui_grid_set_col_flex(grid, 2, 1);

    cgui_window_on_destroy(window, window_destroy_callback);

    cgui_window_push_grid(window, grid);
    cgui_window_rename(window, "crecord");
    cgui_window_resize(window, 480, 360);
    cgui_window_activate(window);

    cgui_run();

    if (cgui_error()) {
        printf("Gui has failed during operation (%i).\n", cgui_error());
    }

    cgui_window_destroy(window);
    cgui_grid_destroy(grid);
    cgui_cell_destroy(label);

    cgui_reset();
}

void label_set_state(enum RECORDING_STATE new_state){
    switch (new_state){
        case STOPPED:
            cgui_label_set(label, "State: STOPPED");
            break;
        case RECORDING:
            cgui_label_set(label, "State: RECORDING");
            break;
        case PAUSED:
            cgui_label_set(label, "State: PAUSED");
            break;

        default: cgui_label_set(label, "State: UNKNOWN");
    }
}

void window_destroy_callback(cgui_window* w){
    if (started_recording == true){
        if (paused_recording == false){
            stop_recording(pid);
        }
        concat_videos(file_names, file_names_length, today);
        free(today);
    }
}

void button_record_callback(cgui_cell* c){
    if (started_recording == false){
        pid = start_recording(&file_names, file_names_length);
        file_names_length++;

        today = get_today();
        started_recording = true;
        cgui_button_enable(button_pause);
        label_set_state(RECORDING);
    }
    else {
        if (paused_recording == false){
            stop_recording(pid);
        }
        concat_videos(file_names, file_names_length, today);
        
        free(today);

        started_recording = false;
        cgui_button_disable(button_pause);
        label_set_state(STOPPED);
    }
}

void button_pause_callback(cgui_cell* c){
    if (started_recording == true){
        if (paused_recording == false){
            stop_recording(pid);
            
            paused_recording = true;
            label_set_state(PAUSED);
        }
        else {
            pid = start_recording(&file_names, file_names_length);
            file_names_length++;
            
            paused_recording = false;
            label_set_state(RECORDING);
        }
    }
}
