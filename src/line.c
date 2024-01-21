/*
 *  Functions for manipulating text lines
 */
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "line.h"
#include "utils.h"

/*
 *  Purpose: Expand the capacity of a Line structure to accommodate additional characters.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to expand.
 *    - n: The number of additional characters to accommodate.
 * 
 *  Returns: None.
 */
void line_expand(Line* line, size_t n)
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
        line->chars = utils_cp(realloc(line->chars, new_capacity * sizeof(line->chars[0])));
        line->capacity = new_capacity;
    }
}

/*
 *  Purpose: Insert a null-terminated string before the cursor position in a Line structure.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to insert text into.
 *    - text: Null-terminated string to insert.
 *    - col: Pointer to the cursor position where the text is inserted.
 *
 *  Returns: None.
 */
void line_insert_text_before_cursor(Line* line, char* text, size_t* col)
{
    line_insert_text_segment_before_cursor(line, text, strlen(text), col);
}

/*
 *  Purpose: Insert a text segment of a specified size before the cursor position in a Line structure.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to insert text into.
 *    - text: Pointer to the text segment to insert.
 *    - text_size: The size of the text segment to insert.
 *    - col: Pointer to the cursor position where the text is inserted.
 *
 *  Returns: None.
 */
void line_insert_text_segment_before_cursor(Line* line, char* text, size_t text_size, size_t* col)
{
    assert(*col <= line->size);
    line_expand(line, text_size);

    char* src = line->chars + *col;
    memmove(src + text_size, src, line->size - *col);
    memcpy(src, text, text_size);
    
    line->size += text_size;
    *col += text_size;
}

/*
 *  Purpose: Check if there are leading whitespace characters in a Line structure before the cursor position.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to check.
 *    - col: Pointer to the cursor position.
 *
 *  Returns:
 *    - true if there are leading whitespace characters.
 *    - false otherwise.
 */
static bool leading_whitespace(const Line* line, size_t* col)
{
    for (size_t i = 0; i < *col; i++) {
        if (line->chars[i] != ' ')
            return false;
    }
    return true;
}

/*
 *  Purpose: Perform a backspace operation in a Line structure at the cursor position.
 *
 *  Parameters:
 *      - line: Pointer to the Line structure to perform the backspace operation.
 *      - col: Pointer to the cursor position.
 *
 *  Returns: None.
 */
void line_backspace(Line* line, size_t* col)
{
    assert(*col <= line->size);
    size_t backspaces = 1;
    if (leading_whitespace(line, col))
        backspaces = ((*col) % TAB_STOP == 0) ? TAB_STOP : (*col) % TAB_STOP;

    char* src = line->chars + *col;
    if (*col > 0) {
        memmove(src - backspaces, src, line->size - *col);
        line->size -= backspaces;
        *col -= backspaces;
    }
}

/*
 *  Purpose: Delete a character at the cursor position in a Line structure.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to perform the delete operation.
 *    - col: Pointer to the cursor position.
 *
 *  Returns: None.
 */
void line_delete(Line* line, size_t* col)
{
    if (*col < line->size) {
        char* src = line->chars + *col + 1;
        memmove(src - 1, src, line->size - *col);
        line->size--;
    }
}

/*
 *  Purpose: Append a null-terminated string to the end of a Line structure.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to append text to.
 *    - text: Null-terminated string to append.
 * 
 *  Returns: None.
 */
void line_append_text(Line* line, char* text)
{
    line_append_text_segment(line, text, strlen(text));
}

/*
 *  Purpose: Append a text segment of a specified size to the end of a Line structure.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to append text to.
 *    - text: Pointer to the text segment to append.
 *    - text_size: The size of the text segment to append.
 * 
 *  Returns: None.
 */
void line_append_text_segment(Line* line, char* text, size_t text_size)
{
    size_t col = line->size;
    line_insert_text_segment_before_cursor(line, text, text_size, &col);
}