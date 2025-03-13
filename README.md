# CTODO — Little *UNFINISHED* program made with cassete cgui

This program is unfinished, and more of a practice for me before making a real app using this library

### What you can find here:
- Input handling that supports only latin characters
- The way to swap grids (because there is no such an example project in original cassette repository </3)
- And other nerdy C array, memory, pointers stuff

### Implemented:
- Changing status (For now only 3 statuses: ND - "Not Done", INP - "In Progress", and D - "Done")
- Deleting ToDo values (Shift + Click on status button)
- Adding new ToDo values
- Renaming (Click on title, Control + Backspace to erase whole text, no way to undo btw :3)
- Saving and loading ToDo values

### Not Implemented:
- Pages (May never implement it, if you want more than 7 values, feel free to fork this repository and change value of ROWS_COUNT at the top of main.c file)
