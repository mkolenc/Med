#ifndef EDITOR_H_
#define EDITOR_H_

#include <stdlib.h>
#include "line.h"

#define EDITOR_INIT_CAPACITY 128



// sequence of lines but also a stretchy buffer as it
// reallocates as you push more lines into it
typedef struct {
    size_t capacity;
    size_t size;
    Line* lines;
    size_t cursor_row;
    size_t cursor_col;
} Editor;

// How I imagine this working is we call eidtor_insert_text with our piece of text,
// It grabs the cursor position and calls line_insert_text with the cursor position and updating
// the cursor postion correspondingly
Editor* editor_init(void);
void editor_free(Editor* editor);

void editor_insert_text_before_cursor(Editor* editor, char* text);
void editor_backspace(Editor* editor);
void editor_delete(Editor* editor);
void editor_left_arrow(Editor* editor);
void editor_right_arrow(Editor* editor);
void editor_up_arrow(Editor* editor);
void editor_down_arrow(Editor* editor);
void editor_return(Editor* editor);
void editor_tab(Editor* editor);
// void editor_insert_new_line(Editor* editor);
// const char* editor_char_under_cursor(Editor* editor);
// void editor_save_to_file(Editor* editor, const char* file_path);


#endif /* EDITOR_H_ */ 