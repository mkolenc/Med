/*
 *  Utility functions for 2D vector (Vec2f) operations.
 */
#ifndef VEC_H_
#define VEC_H_

#include "vec.h"

typedef struct {
    float x, y;
} Vec2f;

/*
 *  Purpose: Create a 2D vector with the specified x and y components.
 *
 *  Parameters:
 *    - x: The x-component of the vector.
 *    - y: The y-component of the vector.
 *
 *  Returns:
 *    - A Vec2f structure representing the 2D vector with the given components.
 */
Vec2f vec2f(float x, float y);

/*
 *  Purpose: Create a 2D vector with the same value for both x and y components.
 *
 *  Parameters:
 *    - x: The value to be used for both x and y components.
 *
 *  Returns:
 *    - A Vec2f structure representing the 2D vector with the same value for x and y components.
 */
Vec2f vec2fs(float x);

/*
 *  Purpose: Add two 2D vectors element-wise.
 *
 *  Parameters:
 *    - a: The first 2D vector to be added.
 *    - b: The second 2D vector to be added.
 *
 *  Returns:
 *    - The result of adding the two input vectors element-wise.
 */
Vec2f vec2f_add(Vec2f a, Vec2f b);

/*
 *  Purpose: Subtract one 2D vector from another element-wise.
 *
 *  Parameters:
 *    - a: The 2D vector from which to subtract.
 *    - b: The 2D vector to subtract.
 *
 *  Returns:
 *    - The result of subtracting the second vector from the first element-wise.
 */
Vec2f vec2f_sub(Vec2f a, Vec2f b);

/*
 *  Purpose: Scales a vector.
 *
 *  Parameters:
 *    - a: The first 2D vector to be scaled.
 *    - b: The value that a is to be scaled by.
 *
 *  Returns:
 *    - The result of a scaled by b.
 */
Vec2f vec2f_scale(Vec2f a, float b);

#endif /* VECTOR_H_ */