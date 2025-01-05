#ifndef _CAMERA_H
#define _CAMERA_H

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Math/LinAlgFunctions.hpp"

namespace Nilib
{

    struct Camera
    {
        Vec3f position = {0.0f, 0.0f, 0.0f};
        Vec3f direction = {1.0f, 0.0f, 0.0f}; // Normalized ?

        // Projection matrix?

        void move_backward()
        {
            position -= 0.1f * direction;
        }

        void move_forward()
        {
            position += 0.1f * direction;
        }
        void move_left()
        {
            // position += 0.1 * Nilib::norm(direction);
        }
        void move_up()
        {
            //;
        }
    };

} // namespace Nilib

#endif