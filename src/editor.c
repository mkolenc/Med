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

static int last_input = 0;

// on the first call, allocates EDItor_INIT_Capacity of lines and then initilisez them to 0
// On future calls, if space is required, doulbes the allocated space and initilizes the new space
// if it isn't required, it does nothing: note, n is the number of new lines we want to add/exapnt to fit
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
        editor->lines = cp(realloc(editor->lines, new_capacity * sizeof(editor->lines[0])));

        for (size_t i = editor->capacity; i < new_capacity; i++)
            line_init_members(&editor->lines[i]);
        editor->capacity = new_capacity;
    }
}

static void editor_push_new_line(Editor* editor)
{
    editor_expand(editor, 1);
    editor->size++;;
}

Editor* editor_init(void)
{
    Editor* editor = cp(malloc(sizeof(*editor)));

    editor->lines = NULL;
    editor->capacity = 0;
    editor->size = 0;
    editor->cursor_col = 0;
    editor->cursor_row = 0;

    return editor;
}

void editor_free(Editor* editor)
{
    for (size_t i = 0; i < editor->size; i++)
        free(editor->lines[i].chars);
    free(editor->lines);
    free(editor);
}

void editor_insert_text_before_cursor(Editor* editor, char* text)
{
    // The cursor_row is 0 based and the size is 1 based, so the 0th row is the first line
    // so if the cursor_row == size, we need to add more lines, or it hasn't been init yet
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);
    line_insert_text_before_cursor(&editor->lines[editor->cursor_row], text, &editor->cursor_col);
    last_input = SDL_TEXTINPUT;
}

void editor_backspace(Editor *editor) 
{
    if (editor->cursor_row == editor->size) // its not shifting ht botom line up
        editor_push_new_line(editor);

    if (editor->cursor_col == 0 && editor->cursor_row > 0) { // multiple lines
        Line* curr_line = editor->lines + editor->cursor_row;
        Line* dst_line = curr_line - 1;

        // copy any trailing characters onto the destination line and update
        size_t trailing_chars = curr_line->size; // cursor_col is 0 so it must be the whole line
        line_expand(dst_line, trailing_chars);
        memcpy(dst_line->chars + dst_line->size, curr_line->chars, trailing_chars);
        dst_line->size += trailing_chars;

        // curr_line data has been copied, chars buffer needs to be freed
        free(curr_line->chars);

        // shift all of the lines up
        size_t lines_to_shift = editor->size - editor->cursor_row - 1;
        memmove(curr_line, curr_line + 1, lines_to_shift * sizeof(*curr_line));

        // The last line has been moved up so we still have its data, need to reset this line
        // so future inserts don't mess up our other lines /or cause segfaults
        line_init_members(editor->lines + editor->size - 1);

        // update the changes to lines and editor
        editor->size--;
        editor->cursor_row--;
        editor->cursor_col = dst_line->size - trailing_chars;
    } else
        line_backspace(&editor->lines[editor->cursor_row], &editor->cursor_col);
    last_input = SDLK_BACKSPACE;
}

void editor_delete(Editor *editor)
{
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);

    if (editor->cursor_col == editor->lines[editor->cursor_row].size && editor->cursor_row < editor->size - 1) {
        Line* curr_line = editor->lines + editor->cursor_row;

        // copy any trailing characters onto the destination line and update
        size_t trailing_chars = (curr_line + 1)->size; // cursor_col is 0 so it must be the whole line
        line_expand(curr_line, trailing_chars);
        memcpy(curr_line->chars + curr_line->size, (curr_line + 1)->chars, trailing_chars);
        curr_line->size += trailing_chars;

        // curr_line data has been copied, chars buffer needs to be freed
        free((curr_line + 1)->chars);

        // shift all of the lines up
        size_t lines_to_shift = editor->size - editor->cursor_row - 2;
        memmove(curr_line + 1, curr_line + 2, lines_to_shift * sizeof(*curr_line));

        // The last line has been moved up so we still have its data, need to reset this line
        // so future inserts don't mess up our other lines /or cause segfaults
        line_init_members(editor->lines + editor->size - 1);

        // update the changes to lines and editor
        editor->size--;
    } else
        line_delete(&editor->lines[editor->cursor_row], &editor->cursor_col);
    last_input = SDLK_DELETE;
}

void editor_left_arrow(Editor* editor)
{
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);

    if (editor->cursor_col > 0)
        editor->cursor_col--;
    else if (editor->cursor_row > 0) {
        editor->cursor_row--;
        editor->cursor_col = editor->lines[editor->cursor_row].size;
    }
    last_input = SDLK_LEFT;
}

void editor_right_arrow(Editor* editor)
{
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);

    if (editor->cursor_col < editor->lines[editor->cursor_row].size) 
        editor->cursor_col++;
    else if (editor->cursor_row < editor->size - 1 ) {
        editor->cursor_row++;
        editor->cursor_col = 0;
    }
    last_input = SDLK_RIGHT;
}

void editor_up_arrow(Editor* editor)
{
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);

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
    
void editor_down_arrow(Editor* editor)
{
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);

    static size_t start_col = 0;

    size_t bottom_line_size = editor->lines[editor->size].size;
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


/* creates a new line, underneath current one, shift chars down if there are to rigth to cursor, works with whitespace indent*/
void editor_return(Editor* editor) {
    // Create space for a new line
    editor_push_new_line(editor);

    // Calculate pointers for the source and destination lines
    Line* curr_line = editor->lines + editor->cursor_row;
    Line* src_line = curr_line + 1;
    Line* dst_line = curr_line + 2;

    // Move lines down
    size_t lines_down = editor->size - editor->cursor_row - 1;
    memmove(dst_line, src_line, lines_down * sizeof(*curr_line));

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

    // Initialize and reallocate the source line
    size_t num_carry_chars = curr_line->size - editor->cursor_col;
    line_init_members(src_line);
    line_expand(src_line, num_carry_chars + indentation);

    // Copy whitespace and characters to the new line
    memset(src_line->chars, ' ', indentation); ///////////////////// this needs to be updated to not just be spaces, but tabs too
    memcpy(src_line->chars + indentation, &curr_line->chars[editor->cursor_col], num_carry_chars);

    // Update line sizes and cursor position
    curr_line->size -= num_carry_chars;
    src_line->size += num_carry_chars + indentation;
    editor->cursor_row++;
    editor->cursor_col = indentation;
}

void editor_tab(Editor* editor)
{
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);

    size_t spaces = TAB_STOP - (editor->cursor_col % TAB_STOP);

    char tab_str[spaces + 1];
    memset(tab_str, ' ', spaces);
    tab_str[spaces] = '\0';

    editor_insert_text_before_cursor(editor, tab_str);
}