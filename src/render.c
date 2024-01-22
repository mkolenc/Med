/*
 *  Contains functions for rendering text and graphical elements using SDL.
 */
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "editor.h"
#include "utils.h"
#include "font.h"
#include "vec.h"
#include "SDL.h"
#include "camera.h"

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
void render_char(SDL_Renderer* renderer, const Font* font, char c, Vec2f pos, float scale)
{
    SDL_Rect dst = {
        .x = pos.x,
        .y = pos.y,
        .w = FONT_WIDTH * scale,
        .h = FONT_HEIGHT * scale,
    };

    size_t index = '?' - ASCII_DISPLAY_LOW;
    if (c >= ASCII_DISPLAY_LOW && c <= ASCII_DISPLAY_HIGH)
        index = c - ASCII_DISPLAY_LOW;

    utils_scc(SDL_RenderCopy(renderer, font->spritesheet, &font->glyphs[index], &dst));
}

/*
 *  Purpose: Set the color of an SDL texture to the specified SDL_Color.
 *
 *  Parameters:
 *    - texture: Pointer to the SDL texture for color modification.
 *    - color: The SDL_Color to set for the texture.
 *
 *  Returns: None.
 */
static void set_texture_color(SDL_Texture* texture, SDL_Color color)
{
    utils_scc(SDL_SetTextureColorMod(texture, color.r, color.g, color.b));
    utils_scc(SDL_SetTextureAlphaMod(texture, color.a));
}

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
void render_text_segment(SDL_Renderer* renderer, const Font* font, const char* text, size_t text_size, Vec2f pos, SDL_Color color, float scale)
{
    set_texture_color(font->spritesheet, color);

    for (size_t i = 0; i < text_size; i++) {
        render_char(renderer, font, text[i], pos, scale);
        pos.x += FONT_WIDTH * scale;
    }
}

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
void render_text(SDL_Renderer* renderer, const Font* font, const char* text, Vec2f pos, SDL_Color color, float scale)
{
    render_text_segment(renderer, font, text, strlen(text), pos, color, scale);
}

/*
 *  Purpose: Retrieve the character under the cursor in the text editor.
 *
 *  Parameters:
 *    - editor: Pointer to the Editor structure containing the text.
 *
 *  Returns:
 *    - Pointer to the character under the cursor, or NULL if the cursor is outside the text bounds.
 */
static char* text_under_cursor(Editor* editor)
{
    if (editor->size == 0)
        return NULL;

    Line* line = editor->lines + editor->cursor_row;
    size_t col = editor->cursor_col;

    return col < line->size ? line->chars + col : NULL;
}

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
void render_editor(SDL_Renderer* renderer, Font* font, Editor* editor, SDL_Window* window, Camera* camera, SDL_Color text_color, float scale)
{
    for (size_t i = 0; i < editor->size; i++) {
        Vec2f line_pos = camera_get_projection_point(vec2f(0.0f, i * FONT_HEIGHT * FONT_SCALE), camera->pos, window);
        render_text_segment(renderer, font, editor->lines[i].chars, editor->lines[i].size, line_pos, text_color, scale);
    }
}

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
void render_cursor(SDL_Renderer* renderer, const Font* font, Editor* editor, SDL_Window* window, Vec2f camera_pos, SDL_Color cursor_color, SDL_Color text_beneath_cursor_color, CursorShape cursor_shape)
{

    Vec2f pos = camera_get_projection_point(vec2f(editor->cursor_col * FONT_WIDTH * FONT_SCALE, editor->cursor_row * FONT_HEIGHT * FONT_SCALE), camera_pos, window);

    switch (cursor_shape) {
        case CURSOR_BOX: {
            SDL_Rect dst = {
                .x = pos.x,
                .y = pos.y,
                .w = FONT_WIDTH * FONT_SCALE,
                .h = FONT_HEIGHT * FONT_SCALE,
            };

            utils_scc(SDL_SetRenderDrawColor(renderer, cursor_color.r, cursor_color.g, cursor_color.b, cursor_color.a));
            utils_scc(SDL_RenderFillRect(renderer, &dst));

            const char* c = text_under_cursor(editor);
            if (c != NULL) {
                set_texture_color(font->spritesheet, text_beneath_cursor_color);
                render_char(renderer, font, *c, vec2f(dst.x, dst.y), FONT_SCALE);
            }
        }
        break;

        case CURSOR_BAR: {
            const int bar_width = 1;

            SDL_Rect dst = {
                .x = pos.x,
                .y = pos.y,
                .w = bar_width * FONT_SCALE,
                .h = FONT_HEIGHT * FONT_SCALE,
            };

            utils_scc(SDL_SetRenderDrawColor(renderer, cursor_color.r, cursor_color.g, cursor_color.b, cursor_color.a));
            utils_scc(SDL_RenderFillRect(renderer, &dst));
        }
        break;

        case CURSOR_UNDERSCORE: {
            const int underscore_height = 2;

            SDL_Rect dst = {
                .x = pos.x,
                .y = pos.y + (FONT_HEIGHT * FONT_SCALE) - underscore_height,
                .w = FONT_WIDTH * FONT_SCALE,
                .h = underscore_height * FONT_SCALE,
            };

            utils_scc(SDL_SetRenderDrawColor(renderer, cursor_color.r, cursor_color.g, cursor_color.b, cursor_color.a));
            utils_scc(SDL_RenderFillRect(renderer, &dst));
        }
        break;
    }
}
