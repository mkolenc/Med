#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "line.h"
#include "editor.h"
#include "line.h"
#include "utils.h"


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
    for (size_t i = 0; i < editor->size; i++) {
        free(editor->lines[i].chars);
        free(&editor->lines[i]);
    }
    free(editor);
}

void editor_insert_text_before_cursor(Editor* editor, char* text)
{
    // The cursor_row is 0 based and the size is 1 based, so the 0th row is the first line
    // so if the cursor_row == size, we need to add more lines, or it hasn't been init yet
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);
    line_insert_text_before_cursor(&editor->lines[editor->cursor_row], text, &editor->cursor_col);
}

void editor_backspace(Editor *editor)
{
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);
    line_backspace(&editor->lines[editor->cursor_row], &editor->cursor_col);
}

void editor_delete(Editor *editor)
{
    if (editor->cursor_row == editor->size)
        editor_push_new_line(editor);
    line_delete(&editor->lines[editor->cursor_row], &editor->cursor_col);
}

// void editor_insert_new_line(Editor *editor)
// {
//     if (editor->cursor_row > editor->size) {
//         editor->cursor_row = editor->size;
//     }

//     editor_grow(editor, 1);

//     const size_t line_size = sizeof(editor->lines[0]);
//     memmove(editor->lines + editor->cursor_row + 1,
//             editor->lines + editor->cursor_row,
//             (editor->size - editor->cursor_row) * line_size);
//     memset(&editor->lines[editor->cursor_row + 1], 0, line_size);
//     editor->cursor_row += 1;
//     editor->cursor_col = 0;
//     editor->size += 1;
// }

// const char *editor_char_under_cursor(const Editor *editor)
// {
//     if (editor->cursor_row < editor->size) {
//         if (editor->cursor_col < editor->lines[editor->cursor_row].size) {
//             return &editor->lines[editor->cursor_row].chars[editor->cursor_col];
//         }
//     }
//     return NULL;
// }

// void editor_save_to_file(const Editor *editor, const char *file_path)
// {
//     FILE *f = fopen(file_path, "w");
//     if (f == NULL) {
//         fprintf(stdout, "ERROR: could not open file `%s`: %s\n",
//                 file_path, strerror(errno));
//         exit(1);
//     }

//     for (size_t row = 0; row < editor->size; ++row) {
//         fwrite(editor->lines[row].chars, 1, editor->lines[row].size, f);
//         fputc('\n', f);
//     }

//     fclose(f);
// }
