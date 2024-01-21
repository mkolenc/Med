#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "line.h"
#include "utils.h"

#define LINE_INIT_CAPACITY 1024

void line_init_members(Line* line)
{
    line->capacity = 0;
    line->size = 0;
    line->chars = NULL;
}

static void line_expand(Line* line, size_t n)
{
    size_t new_capacity = line->capacity;

    assert(new_capacity >= line->size);
    while (new_capacity - line->size < n) { // free space is less than text to add
        if (new_capacity == 0)
            new_capacity = LINE_INIT_CAPACITY;
        else
            new_capacity *= 2;
    }

    if (new_capacity != line->capacity) {
        line->chars = cp(realloc(line->chars, new_capacity)); // realloc might fail, return null, still nead to release line->buffrer
        line->capacity = new_capacity;
    }
}

void line_insert_text_before_cursor(Line* line, char* text, size_t* col)
{
    const size_t text_size = strlen(text);
    line_expand(line, text_size);

    memmove(line->chars + *col + text_size,
            line->chars + *col, 
            line->size - *col);
    memcpy(line->chars + *col, text, text_size);
    line->size += text_size;
    *col += text_size;
}

void line_backspace(Line* line, size_t* col)
{
    if (*col > 0 && line->size > 0) { // do i really nead both of these conditions? no, only col > 0
        memmove(line->chars + *col - 1,
                line->chars + *col,
                line->size - *col);
        line->size--;
        *col -= 1;
    }
}

void line_delete(Line* line, size_t* col)
{
    if (*col < line->size && line->size > 0) {
        memmove(line->chars + *col,
                line->chars + *col + 1,
                line->size - *col);
        line->size--;
    }
}