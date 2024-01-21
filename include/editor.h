/*
*  The operations and data structure of the text editor.
*/
#ifndef EDITOR_H_
#define EDITOR_H_

#include "line.h"

#define EDITOR_INIT_CAPACITY 128

// TODO: when saving files, convert TAB_STOP spaces into  a '\t' char to save space, then fix the save file function to correct for this


// Sequence of lines but actually a stretchy buffer as it
// reallocates as you push more lines onto it
typedef struct {
    size_t capacity;
    size_t size;
    Line* lines;
    size_t cursor_row;
    size_t cursor_col;
} Editor;

/*
 *  Purpose: Insert text before the cursor in the Editor's current line.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 *    - text: The text to be inserted.
 * 
 *  Returns: None.
 */
void editor_insert_text_before_cursor(Editor* editor, char* text);

/*
 *  Purpose: Delete the character before the cursor in the editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_backspace(Editor *editor);

/*
 *  Purpose: Delete the character after the cursor in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_delete(Editor *editor);

/*
 *  Purpose: Move the cursor one position to the left in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_left_arrow(Editor* editor);

/*
 *  Purpose: Move the cursor one position to the right in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_right_arrow(Editor* editor);

/*
 *  Purpose: Move the cursor one line up in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_up_arrow(Editor* editor);

/*
 *  Purpose: Move the cursor one line down in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_down_arrow(Editor* editor);

/*
 *  Purpose: Create a new line underneath the current one, shifting characters as needed with whitespace indentation.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_return(Editor* editor);
/*
 *  Purpose: Insert a tab character at the cursor position in the Editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_tab(Editor* editor);

/*
 *  Purpose: Save the contents of the Editor to a file at the specified file path.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 *    - file_path: The path to the file where the contents will be saved.
 * 
 *  Returns: None.
 */
void editor_save_to_file(const Editor* editor, const char* file_path);

/*
 *  Purpose: Load text data from a file and populate the Editor with its contents.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure to populate.
 *    - fp: File pointer to the file from which to read the contents.
 * 
 *  Returns: None.
 */
void editor_load_from_file(Editor* editor, FILE* fp);

/*
 *  Purpose: Free the memory allocated for the Editor's lines and associated data.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure.
 * 
 *  Returns: None.
 */
void editor_free(Editor* editor);

#endif /* EDITOR_H_ */ 