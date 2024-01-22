/*
 *  Functions for manipulating text lines.
 *  These functions are designed to workd with lines that have been zero-initialized.
 *  The lines should be freed using 'line_free' when they are no longer needed.
 */
#ifndef LINE_H_
#define LINE_H_

#include <stdlib.h>
#include "line.h"

#define LINE_INIT_CAPACITY 1024
#define TAB_STOP 4

typedef struct {
    size_t capacity;
    size_t size;
    char* chars;
} Line;

/*
 *  Purpose: Expand the capacity of a Line structure to accommodate additional characters.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to expand.
 *    - n: The number of additional characters to accommodate.
 * 
 *  Returns: None.
 */
void line_expand(Line* line, size_t n);

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
void line_insert_text_before_cursor(Line* line, char* text, size_t* col);

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
void line_insert_text_segment_before_cursor(Line* line, char* text, size_t text_size, size_t* col);

/*
 *  Purpose: Perform a backspace operation in a Line structure at the cursor position.
 *
 *  Parameters:
 *      - line: Pointer to the Line structure to perform the backspace operation.
 *      - col: Pointer to the cursor position.
 *
 *  Returns: None.
 */
void line_backspace(Line* line, size_t* col);

/*
 *  Purpose: Delete a character at the cursor position in a Line structure.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to perform the delete operation.
 *    - col: Pointer to the cursor position.
 *
 *  Returns: None.
 */
void line_delete(Line* line, size_t* col);

/*
 *  Purpose: Append a null-terminated string to the end of a Line structure.
 *
 *  Parameters:
 *    - line: Pointer to the Line structure to append text to.
 *    - text: Null-terminated string to append.
 * 
 *  Returns: None.
 */
void line_append_text(Line* line, char* text);

/*
 *  Purpose: Append a text segment of a specified size to the end of a Line structure.
 *
 *  Preconditions: The size of the text segment (text_size) must not exceed the length of the text. 
 *  Note: The implementation allows for non-null-terminated char arrays.
 * 
 *  Parameters:
 *    - line: Pointer to the Line structure to append text to.
 *    - text: Pointer to the text segment to append.
 *    - text_size: The size of the text segment to append.
 * 
 *  Returns: None.
 */
void line_append_text_segment(Line* line, char* text, size_t text_size);

#endif /* LINE_H_ */