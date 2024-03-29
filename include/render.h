/*
 *  Contains functions for rendering text and graphical elements using SDL.
 */
#ifndef RENDER_H_
#define RENDER_H_

#include "SDL.h"
#include "font.h"
#include "editor.h"
#include "vec.h"
#include "camera.h"

typedef enum {
      CURSOR_BAR,
      CURSOR_BOX,
      CURSOR_UNDERSCORE,
  } CursorShape; // this doesn't belong in here, but is for now

/*
 *  Purpose: Render a single character to the window using a specified font and position.
 *
 *  Parameters:
 *    - renderer: Pointer to the SDL renderer where the character is rendered.
 *    - font: Pointer to the Font structure used for rendering.
 *    - c: The character to be rendered.
 *    - pos: The position (Vec2f) where the character is rendered.
 *    - scale: The scaling factor for the character's size.
 *
 *  Returns: None.
 */
void render_char(SDL_Renderer* renderer, const Font* font, char c, Vec2f pos, float scale);

/*
 *  Purpose: Render a substring of text on the screen using a specified font, color, and position.
 *
 *  Preconditions: The size of the text segment (text_size) must not exceed the length of the text. 
 *  Note: The implementation allows for non-null-terminated char arrays.
 *
 *  Parameters:
 *    - renderer: Pointer to the SDL renderer where the text is rendered.
 *    - font: Pointer to the Font structure used for rendering.
 *    - text: Pointer to the text segment to be rendered.
 *    - text_size: The size of the text segment.
 *    - pos: The position (Vec2f) where the text is rendered.
 *    - color: The color for the rendered text.
 *    - scale: The scaling factor for the text's size.
 *
 *  Returns: None.
 */
void render_text_segment(SDL_Renderer* renderer, const Font* font, const char* text, size_t text_size, Vec2f pos, SDL_Color color, float scale);

/*
 *  Purpose: Render a full null-terminated string on the screen using a specified font, color, and position.
 *
 *  Parameters:
 *    - renderer: Pointer to the SDL renderer where the text is rendered.
 *    - font: Pointer to the Font structure used for rendering.
 *    - text: Pointer to the null-terminated string to be rendered.
 *    - pos: The position (Vec2f) where the text is rendered.
 *    - color: The color for the rendered text.
 *    - scale: The scaling factor for the text's size.
 *
 *  Returns: None.
 */
void render_text(SDL_Renderer* renderer, const Font* font, const char* text, Vec2f pos, SDL_Color color, float scale);

/*
 *  Purpose: Render text lines in the editor using camera projection.
 *
 *  Parameters:
 *    - renderer: Pointer to the SDL renderer for rendering operations.
 *    - font: Pointer to the Font structure for rendering text.
 *    - editor: Pointer to the Editor structure containing text lines to be rendered.
 *    - window: Pointer to the SDL window to determine screen dimensions.
 *    - camera: Pointer to the Camera structure for camera projection adjustment.
 *    - text_color: SDL_Color specifying the color of the rendered text.
 *    - scale: Scaling factor for adjusting the text size.
 *
 *  Returns: None.
 */
void render_editor(SDL_Renderer* renderer, Font* font, Editor* editor, SDL_Window* window, Camera* camera, SDL_Color text_color, float scale);

/*
 *  Purpose: Render the cursor at the current cursor position in the text editor using specified colors.
 *
 *  Parameters:
 *    - renderer: Pointer to the SDL renderer where the cursor is rendered.
 *    - font: Pointer to the Font structure for rendering text beneath the cursor.
 *    - editor: Pointer to the Editor structure containing the text and cursor position.
 *    - cursor_color: The color for rendering the cursor.
 *    - text_beneath_cursor_color: The color for rendering text beneath the cursor.
 *
 *  Returns: None.
 */
void render_cursor(SDL_Renderer* renderer, const Font* font, Editor* editor, SDL_Window* window, Vec2f camera_pos, SDL_Color cursor_color, SDL_Color text_beneath_cursor_color, CursorShape cursor_shape);

#endif /* RENDER_H_ */