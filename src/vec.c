/*
 *  Utility functions for 2D vector (Vec2f) operations.
 */
#include "vec.h"

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
Vec2f vec2f(float x, float y)
{
    return (Vec2f) {.x = x, .y = y,};
}

/*
 *  Purpose: Create a 2D vector with the same value for both x and y components.
 *
 *  Parameters:
 *    - x: The value to be used for both x and y components.
 *
 *  Returns:
 *    - A Vec2f structure representing the 2D vector with the same value for x and y components.
 */
Vec2f vec2fs(float x)
{
    return vec2f(x, x);
}

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
Vec2f vec2f_add(Vec2f a, Vec2f b)
{
    return vec2f(a.x + b.x, a.y + b.y);
}

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
Vec2f vec2f_sub(Vec2f a, Vec2f b)
{
    return vec2f(a.x - b.x, a.y - b.y);
}

/*
 *  Purpose: Multiply two 2D vectors element-wise.
 *
 *  Parameters:
 *    - a: The first 2D vector to be multiplied.
 *    - b: The second 2D vector to be multiplied.
 *
 *  Returns:
 *    - The result of multiplying the two input vectors element-wise.
 */
Vec2f vec2f_mul(Vec2f a, Vec2f b)
{
    return vec2f(a.x * b.x, a.y * b.y);
}

/*
 *  Purpose: Divide one 2D vector by another element-wise.
 *
 *  Parameters:
 *    - a: The 2D vector to be divided.
 *    - b: The 2D vector by which to divide.
 *
 *  Returns:
 *    - The result of dividing the first vector by the second element-wise.
 */
Vec2f vec2f_div(Vec2f a, Vec2f b)
{
    return vec2f(a.x / b.x, a.y / b.y);
}