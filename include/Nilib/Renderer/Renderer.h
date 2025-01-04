#ifndef _RENDERER_H
#define _RENDERER_H

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Math/Stats.hpp"

namespace Nilib
{
    class Renderer
    {

        
    public:
        struct TriangleVertex 
        {
            Vec3f position;
            Vec4f color;
            // Texture coordinate. 
        };

        Renderer();
        ~Renderer();

        // Adds the triangle to the triangle vertex array. 
        //void drawSimpleTriangle(Vec3f position, Vec4f color);
        void submitTriangle(TriangleVertex const &v1, TriangleVertex const &v2, TriangleVertex const &v3);

        void bindTriangles() const;

        void drawCalls();

    private:
        // For bound detection. 
        RunningStats submitted_x, submitted_y, submitted_z;

        static size_t const maxTriangles = 128;
        size_t triangle_count = 0;
        TriangleVertex triangle_vertices[maxTriangles];

        unsigned int VBO_triangles, VAO_triangles;

        
    };
} // namespace Nilib



#endif