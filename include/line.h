#ifndef LINE_H_
#define LINE_H_

#include <stdlib.h>

#define LINE_INIT_CAPACITY 1024

typedef struct {
    size_t capacity;
    size_t size;
    char* chars;
} Line;

void line_insert_text(Line* line, char* text, size_t col);
void line_backspace(Line* line, size_t col);
void line_delete(Line* line, size_t col);

#endif /* LINE_H_ */