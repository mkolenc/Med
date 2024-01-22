/*
 *  Aims to enhance text scrolling with a virtual camera for smoother movement.
 *  Unlike traditional scrolling, it provides fluid, dynamic text motion.
 */
#include "camera.h"
#include "editor.h"
#include "font.h"
#include "SDL.h" // Uint32

/*
 *  Purpose: Update the camera's position to smoothly follow the cursor in a text editor.
 *
 *  Parameters:
 *    - camera: Pointer to the Camera structure to be updated.
 *    - editor: Pointer to the Editor structure containing cursor position information.
 *
 *  Returns: None.
 */
void camera_update(Camera* camera, Editor* editor)
{
    // Calculate the cursor's position in screen space.
    Vec2f cursor_pos = vec2f(editor->cursor_col * FONT_WIDTH * FONT_SCALE, editor->cursor_row * FONT_HEIGHT * FONT_SCALE);

    // Determine the camera's velocity, which points toward the cursor.
    camera->vel = vec2f_sub(cursor_pos, camera->pos);

    // Update the camera's position to approach the cursor smoothly.
    // The update rate is adjusted for consistent movement regardless of FPS.
    camera->pos = vec2f_add(camera->pos, vec2f_scale(camera->vel, FRAME_TARGET_TIME_S * DEFAULT_CAMERA_SPEED));
}

/*
 *  Purpose: Scale the camera's velocity to adjust its movement speed. A larger scale value 
 *           increases the speed, while a smaller scale value decreases it.
 *
 *  Parameters:
 *    - camera: Pointer to the Camera structure to be modified.
 *    - scale: Scaling factor to adjust the camera's speed (1.0 maintains the current speed).
 *
 *  Returns: None.
 */
void camera_scale_speed(Camera* camera, float scale)
{
    camera->vel = vec2f_scale(camera->vel, scale);
}

/*
 *  Purpose: Cap the frame rate to ensure consistent timing between frames.
 *
 *  Parameters:
 *    - frame_delta_time_ms: Time in milliseconds since the previous frame was rendered.
 *
 *  Returns: None.
 */
void camera_cap_fps(Uint32 frame_delta_time_ms)
{
    Uint32 frame_target_time_ms = FRAME_TARGET_TIME_S * 1000;
    if (frame_delta_time_ms < frame_target_time_ms)
        SDL_Delay(frame_target_time_ms - frame_delta_time_ms);
}

/*
 *  Purpose: Retrieve the dimensions of the SDL window.
 *
 *  Parameters:
 *    - window: Pointer to the SDL window to retrieve dimensions from.
 *
 *  Returns: Vec2f containing the window's width and height.
 */

static Vec2f get_window_size(SDL_Window* window)
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return vec2f(w, h);
}

/*
 *  Purpose: Calculate the screen position to render a point with camera scroll adjustment.
 *
 *  Parameters:
 *    - point: The original point for rendering.
 *    - camera_pos: The current position of the camera.
 *    - window: Pointer to the SDL window to render to retrieve the dimentions.
 *
 *  Returns: Adjusted screen position for rendering.
 */

Vec2f camera_get_projection_point(Vec2f point, Vec2f camera_pos, SDL_Window* window)
{
    return vec2f_add(vec2f_sub(point, camera_pos), vec2f_scale(get_window_size(window), 0.5f));
}