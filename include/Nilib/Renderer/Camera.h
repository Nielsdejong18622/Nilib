#ifndef _CAMERA_H
#define _CAMERA_H

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Math/LinAlgFunctions.hpp"

namespace Nilib
{

    struct Camera
    {
        Vec3f position = {0.0f, 0.0f, 0.0f};
        Vec3f direction = {0.0f, 0.0f, -1.0f}; // Normalized ?

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
            position.x() -= 0.1f;
        }
        void move_right()
        {
            position.x() += 0.1f;
        }
        void move_up()
        {
            position.y() += 0.1f;
        }
        void move_down()
        {
            position.y() -= 0.1f;
        }
    };

} // namespace Nilib

#endif