#ifndef LINE_H_
#define LINE_H_

#include <stdlib.h>
#include "line.h"

#define LINE_INIT_CAPACITY 1024

// array of characters but actually a stretchy buffer
// as it reallocates as you push more characters into it
typedef struct {
    size_t capacity;
    size_t size;
    char* chars;
} Line;

void line_init_members(Line* line);
void line_insert_text_before_cursor(Line* line, char* text, size_t* col);
void line_backspace(Line *line, size_t* col);
void line_delete(Line *line, size_t* col);

#endif /* LINE_H_ */