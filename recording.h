#include <stdlib.h>

// struct date {
//     int year;
//     int month;
//     int day;
//     int hour;
//     int minute;
//     int second;
// };

char* strconcat(const char* string1, const char* string2);

char* get_today();
char* get_default_output();

int start_recording(char*** segments_array, size_t segments_array_size);
void stop_recording(int pid);
void concat_videos(char** segments, size_t segments_length, char* time_started);
