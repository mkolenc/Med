/*
*  The operations and data structure of the text editor.
*/
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "editor.h"
#include "line.h"
#include "utils.h"
#include "SDL.h"

static int last_input = SDLK_UNKNOWN;

/*
 *  Purpose: Expand the capacity of a Editor structure to accommodate additional lines.
 *
 *  Parameters:
 *    - editor: Pointer to the editor structure to expand.
 *    - n: The number of additional lines to accommodate.
 * 
 *  Returns: None.
 */
static void editor_expand(Editor* editor, size_t n)
{
    size_t new_capacity = editor->capacity;
    assert(new_capacity >= editor->size);

    while (new_capacity - editor->size < n) // while free_space < n new lines
        if (new_capacity == 0)
            new_capacity = EDITOR_INIT_CAPACITY;
        else
            new_capacity *= 2;

    if (new_capacity != editor->capacity) {
        editor->lines = utils_cp(realloc(editor->lines, new_capacity * sizeof(editor->lines[0])));

        size_t old_capacity = editor->capacity;
        memset(editor->lines + old_capacity, 0, (new_capacity - old_capacity) * sizeof(editor->lines[0]));
        editor->capacity = new_capacity;
    }
}

/*
 *  Purpose: Push a new line to the Editor structure, expanding its capacity if necessary.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure to which the new line will be added.
 * 
 *  Returns: None.
 */
static void editor_push_new_line(Editor* editor)
{
    editor_expand(editor, 1);
    editor->size++;;
}

/*
 *  Purpose: Ensure that the first line is initialized when the editor is empty. This function 
 *           needs to be called before your first line operation.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure to check and initialize if necessary.
 * 
 *  Returns: None.
 */
static void editor_handle_first_line(Editor* editor)
{
    if (editor->size == 0)
        editor_push_new_line(editor);
}

/*
 *  Purpose: Insert text before the cursor in the Editor's current line.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 *    - text: The text to be inserted.
 * 
 *  Returns: None.
 */
void editor_insert_text_before_cursor(Editor* editor, char* text)
{
    editor_handle_first_line(editor);
    line_insert_text_before_cursor(&editor->lines[editor->cursor_row], text, &editor->cursor_col);
    last_input = SDL_TEXTINPUT;
}

/*
 *  Purpose: Delete the character before the cursor in the editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_backspace(Editor *editor) 
{
    editor_handle_first_line(editor);

    // If cursor is at the start of a line (not the first line), move text to the previous line.
    if (editor->cursor_col == 0 && editor->cursor_row > 0) {
        Line* curr_line = editor->lines + editor->cursor_row;
        Line* prev_line = curr_line - 1;

        // Copy characters from the current line to the previous line.
        size_t num_copy_chars = curr_line->size;
        line_expand(prev_line, num_copy_chars);
        memcpy(prev_line->chars + prev_line->size, curr_line->chars, num_copy_chars);
        prev_line->size += num_copy_chars;

        // Free the char buffer of the current line since its data has been copied.
        free(curr_line->chars);

        // Shift all lines up to fill the gap.
        size_t lines_to_shift = editor->size - editor->cursor_row - 1;
        memmove(curr_line, curr_line + 1, lines_to_shift * sizeof(*curr_line));

        // Reset the last line to ensure future operations work correctly.
        memset(editor->lines + editor->size - 1, 0, sizeof(*curr_line));

        // update the line and editor data
        editor->size--;
        editor->cursor_row--;
        editor->cursor_col = prev_line->size - num_copy_chars;
    } else {
        // If not at the start of a line, perform a regular backspace within the line.
        line_backspace(editor->lines + editor->cursor_row, &editor->cursor_col);
    }

    last_input = SDLK_BACKSPACE; 
}

/*
 *  Purpose: Delete the character after the cursor in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_delete(Editor *editor)
{
    editor_handle_first_line(editor);

    if (editor->cursor_col == editor->lines[editor->cursor_row].size && editor->cursor_row < editor->size - 1) {
        // If cursor is at the end of a line and not the last line in the file, move text from the next line to the current line.
        Line* curr_line = editor->lines + editor->cursor_row;
        Line* next_line = curr_line + 1;

        // Copy characters from the next line to the end of the current line.
        size_t num_copy_chars = next_line->size;
        line_expand(curr_line, num_copy_chars);
        memcpy(curr_line->chars + curr_line->size, next_line->chars, num_copy_chars);
        curr_line->size += num_copy_chars;

        // Free the char buffer of the next line since its data has been copied.
        free(next_line->chars);

        // Shift all lines up to fill the gap.
        size_t lines_to_shift = editor->size - editor->cursor_row - 2;
        memmove(next_line, next_line + 1, lines_to_shift * sizeof(*curr_line));

        // Reset the last line to ensure future operations work correctly.
        memset(editor->lines + editor->size - 1, 0, sizeof(*curr_line));

        // update the changes to lines and editor
        editor->size--;
    } else {
        // If not at the end of a line, perform a regular delete operation within the line.
        line_delete(editor->lines + editor->cursor_row, &editor->cursor_col);
    }

    last_input = SDLK_DELETE;
}
/*
 *  Purpose: Move the cursor one position to the left in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_left_arrow(Editor* editor)
{
    editor_handle_first_line(editor);

    if (editor->cursor_col > 0)
        editor->cursor_col--;
    else if (editor->cursor_row > 0) {
        editor->cursor_row--;
        editor->cursor_col = editor->lines[editor->cursor_row].size;
    }
    last_input = SDLK_LEFT;
}

/*
 *  Purpose: Move the cursor one position to the right in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_right_arrow(Editor* editor)
{
    editor_handle_first_line(editor);

    if (editor->cursor_col < editor->lines[editor->cursor_row].size) 
        editor->cursor_col++;
    else if (editor->cursor_row < editor->size - 1 ) {
        editor->cursor_row++;
        editor->cursor_col = 0;
    }
    last_input = SDLK_RIGHT;
}

/*
 *  Purpose: Move the cursor one line up in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_up_arrow(Editor* editor)
{
    editor_handle_first_line(editor);

    static size_t start_col = 0;

    if (editor->cursor_row == 0)
        editor->cursor_col = 0;
    else {
        size_t new_line_size = editor->lines[editor->cursor_row - 1].size;
        editor->cursor_row--;

        if (last_input == SDLK_UP) {
            editor->cursor_col = (new_line_size >= start_col) ? start_col : new_line_size;
        } else {
            start_col = editor->cursor_col;
            editor->cursor_col = (new_line_size >= start_col) ? start_col : new_line_size;
        }
        last_input = SDLK_UP;
    }
}

/*
 *  Purpose: Move the cursor one line down in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_down_arrow(Editor* editor)
{
    editor_handle_first_line(editor);

    static size_t start_col = 0;

    size_t bottom_line_size = editor->lines[editor->size - 1].size;
    if (editor->cursor_row == editor->size - 1)
        editor->cursor_col = bottom_line_size;
    else {
        size_t new_line_size = editor->lines[editor->cursor_row + 1].size;
        editor->cursor_row++;

        if (last_input == SDLK_DOWN) {
            editor->cursor_col = (new_line_size >= start_col) ? start_col : new_line_size;
        } else {
            start_col = editor->cursor_col;
            editor->cursor_col = (new_line_size >= start_col) ? start_col : new_line_size;
        }
        last_input = SDLK_DOWN;
    }
}

/*
 *  Purpose: Create a new line underneath the current one, shifting characters as needed with whitespace indentation.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_return(Editor* editor)
{
    // Create space for a new line
    editor_push_new_line(editor);

    // Calculate pointers for the source and destination lines
    Line* curr_line = editor->lines + editor->cursor_row;
    Line* next_line = curr_line + 1;

    // Move lines down, making space for the new line
    size_t lines_down = editor->size - editor->cursor_row - 1;
    memmove(next_line + 1, next_line, lines_down * sizeof(*curr_line));

    // Calculate the number of characters for whitespace indentation
    size_t indentation = 0;
    for (size_t i = 0; i < curr_line->size; i++) {
        char ch = curr_line->chars[i];
        if (ch == ' ')
            indentation++;
        else if (ch == '\t')
            indentation += TAB_STOP;
        else
            break;
    }

    // Initialize and reallocate the next line
    size_t num_copy_chars = curr_line->size - editor->cursor_col;
    memset(next_line, 0, sizeof(*next_line));
    line_expand(next_line, num_copy_chars + indentation);

    // Copy whitespace and characters to the new line
    memset(next_line->chars, ' ', indentation);
    memcpy(next_line->chars + indentation, curr_line->chars + editor->cursor_col, num_copy_chars);

    // Update line sizes and cursor position
    curr_line->size -= num_copy_chars;
    next_line->size += num_copy_chars + indentation;
    editor->cursor_row++;
    editor->cursor_col = indentation;
}

/*
 *  Purpose: Insert a tab character at the cursor position in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_tab(Editor* editor)
{
    editor_handle_first_line(editor);

    size_t num_spaces = TAB_STOP - (editor->cursor_col % TAB_STOP);

    char tab_str[num_spaces + 1];
    memset(tab_str, ' ', num_spaces);
    tab_str[num_spaces] = '\0';

    editor_insert_text_before_cursor(editor, tab_str);
}

/*
 *  Purpose: Save the contents of the Editor to a file at the specified file path.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 *    - file_path: The path to the file where the contents will be saved.
 * 
 *  Returns: None.
 */
void editor_save_to_file(const Editor* editor, const char* file_path)
{
    FILE* fp = fopen(file_path, "w");
    if (fp == NULL) {
        fprintf(stdout, "ERROR: could not open file `%s`: %s\n", file_path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (size_t row = 0; row < editor->size; row++) {
        size_t bytes_to_write = editor->lines[row].size;
        if (fwrite(editor->lines[row].chars, 1, bytes_to_write, fp) != bytes_to_write)
            fputs("Error saving file", fp);
        if (row != editor->size - 1)
            fputc('\n', fp);
    }
    fclose(fp);
}

/*
 *  Purpose: Load text data from a file and populate the Editor with its contents.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure to populate.
 *    - fp: File pointer to the file from which to read the contents.
 * 
 *  Returns: None.
 */
void editor_load_from_file(Editor* editor, FILE* fp)
{
    // Ensure that the editor is empty before loading a file
    assert(editor->lines == NULL && "Can only load files into an empty editor...for now");

    static char chunk[EDITOR_INIT_CAPACITY * LINE_INIT_CAPACITY];
    bool new_line = true;

    while (!feof(fp)) {
        size_t bytes_read = fread(chunk, 1, EDITOR_INIT_CAPACITY * LINE_INIT_CAPACITY, fp);
        char* line_start = chunk;
        char* line_end = memchr(chunk, '\n', bytes_read); // Similar to strchr but bounded by bytes_read

        while (line_end != NULL) { // A new line to process
            if (new_line)
                editor_push_new_line(editor);

            Line* current_line = editor->lines + editor->size - 1;

            size_t text_size = line_end - line_start;
            line_append_text_segment(current_line, line_start, text_size);

            bytes_read -= text_size + 1;
            line_start = line_end + 1;
            line_end = memchr(line_start, '\n', bytes_read);
            new_line = true;
        }

        // If there are characters remaining on the current line, append them
        if (bytes_read > 0) {
            if (new_line)
                editor_push_new_line(editor);

            Line* current_line = editor->lines + editor->size - 1;
            line_append_text_segment(current_line, line_start, bytes_read);
            new_line = false;
        }
    }
}

/*
 *  Purpose: Free the memory allocated for the Editor's lines and associated data.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_free(Editor* editor)
{
    for (size_t i = 0; i < editor->size; i++)
        free(editor->lines[i].chars);
    free(editor->lines);
}