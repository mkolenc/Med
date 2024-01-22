/*
 *  Aims to enhance text scrolling with a virtual camera for smoother movement.
 *  Unlike traditional scrolling, it provides fluid, dynamic text motion.
 */
#ifndef CAMERA_H_
#define CAMERA_H_

#define FPS 60
#define FRAME_TARGET_TIME_S (1.0f / FPS)

#define DEFAULT_CAMERA_SPEED 2

#include "vec.h"
#include "editor.h"
#include "SDL.h" // Uint32

typedef struct {
    Vec2f pos, vel;
} Camera;

/*
 *  Purpose: Update the camera's position to smoothly follow the cursor in a text editor.
 *
 *  Parameters:
 *    - camera: Pointer to the Camera structure to be updated.
 *    - editor: Pointer to the Editor structure containing cursor position information.
 *
 *  Returns: None.
 */
void camera_update(Camera* camera, Editor* editor);

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
void camera_scale_speed(Camera* camera, float scale);

/*
 *  Purpose: Cap the frame rate to ensure consistent timing between frames.
 *
 *  Parameters:
 *    - frame_delta_time_ms: Time in milliseconds since the previous frame was rendered.
 *
 *  Returns: None.
 */
void camera_cap_fps(Uint32 frame_delta_time_ms);

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

Vec2f camera_get_projection_point(Vec2f point, Vec2f camera_pos, SDL_Window* window);

#endif /* CAMERA_H_ */