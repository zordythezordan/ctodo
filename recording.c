#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>

#include "recording.h"

// for start_recording(uint_16) function, so I have a fixed size of maximum segments value
// and i read on internet that size_t is uint16_t so the max size of it is 65535 which is 5 symbols
#define MAX_SYMBOLS_IN_SEGMENTS 5

char** strarray_append(char** array, const size_t array_len, const char* string){
    char** new_array = realloc(array, (array_len + 1) * sizeof(char*));
    new_array[array_len] = strdup(string);
    return new_array;
}

char* strconcat(const char* string1, const char* string2){
    char* new_string = malloc((strlen(string1) + strlen(string2)) * sizeof(char));
    strcpy(new_string, string1);
    strcat(new_string, string2);
    
    return new_string;
}

char* get_default_output(){
    FILE* command = popen("pactl info | grep \"Default Sink\" | cut -d ' ' -f 3", "r");

    char buffer[64];
    char* output_name = NULL;

    while(fgets(buffer, sizeof(buffer), command)){
        output_name = strdup(buffer);
        break;
    }

    pclose(command);

    int new_line = strchr(output_name, '\n') - output_name;
    output_name[new_line] = '\0';

    return strconcat(output_name, ".monitor");
}

int start_recording(char*** segments_array, size_t segments_array_size) {
    pid_t* pid = malloc(sizeof(pid_t));
    
    char* array_size_string = malloc(MAX_SYMBOLS_IN_SEGMENTS * sizeof(char));
    snprintf(array_size_string, MAX_SYMBOLS_IN_SEGMENTS, "%lu", segments_array_size + 1);
    
    char* file_name = strconcat("/tmp/crecord_recording_", strconcat(array_size_string, ".mp4"));
    *segments_array = strarray_append(*segments_array, segments_array_size, file_name);

    if (pid == NULL) {
        perror("malloc");
        return -1;
    }

    *pid = fork();

    if (*pid < 0){
        perror("fork");
        free(pid);
        return -1;
    } else if (*pid == 0) {
        execlp("wf-recorder", "-r", "90", "-c", "libx264", "-f", file_name, strconcat("-a", get_default_output()), "-y", NULL);
    }

    return *pid;
}

void stop_recording(const int pid){
    kill(pid, SIGTERM);
}

char* extract_file_name(const char* path){
    char* last_pos = strrchr(path, '/');
    if (last_pos == NULL)
        return NULL;

    size_t path_length = strlen(path);
    int last_slash_index = last_pos - path;
    if ((last_slash_index + 1) == path_length)
        return NULL;

    int file_name_size = path_length - (last_slash_index);
    char* file_name = malloc(file_name_size);
    memcpy(file_name, last_pos + 1, file_name_size);
   
    return file_name;
}

char* get_today(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char* today_string = malloc(20);
    sprintf(today_string, "%d-%02d-%02d_%02d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    today_string[20] = '\0';

    return today_string;
}

void concat_videos(char** segments, size_t segments_length, char* time_started){
    // not making a check for segments_length == 1 on purpose, because when i tried to move the file (all possible ways), only reconverting file with ffmpeg worked
    // so, at this point im just gonna concat 1 file into 1 file :sob:
    FILE* segments_file = fopen("/tmp/crecord_segments", "w");
    
    for (int i = 0; i < segments_length; i++){
        fprintf(segments_file, "file '%s'\n", segments[i]);
    }
    fclose(segments_file);

    char* command = strconcat("ffmpeg -f concat -safe 0 -i /tmp/crecord_segments -c copy ", strconcat(getenv("HOME"), strconcat("/Videos/crecording_", strconcat(time_started, ".mp4"))));
    printf("%s\n", command);

    if (system(command) == -1){
        perror("system");
    }
    else {
        printf("Successfully recorded a video\n");

        for (int i = 0; i < segments_length; i++){
            remove(segments[i]);
        }

        remove("/tmp/crecord_segments");
    }
}
