#ifndef _RENDERER_H
#define _RENDERER_H

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Math/Stats.hpp"
#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Renderer/ShaderProgram.h"
#include "Nilib/Renderer/Camera.h"

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
            // Whatever more.

            static TriangleVertex RandomView()
            {
                return {.position = Nilib::Vec3f::randunif(-1.0f, 1.0f),
                        .color = Color::RandomHSL()};
            }
        };

        Renderer();
        ~Renderer();

        // Adds the triangle to the triangle vertex array.
        // void drawSimpleTriangle(Vec3f position, Vec4f color);
        void submitTriangle(TriangleVertex const &v1, TriangleVertex const &v2, TriangleVertex const &v3);
        void submitQuad(TriangleVertex const &v1, TriangleVertex const &v2, TriangleVertex const &v3, TriangleVertex const &v4);

        void startScene(Window const &window) const;
        void endScene() const;

        void drawCalls();

        void toggleWireFrameMode();

        ShaderProgram triangle_shader;
        ShaderProgram quad_shader;

        Camera camera;

    private:
        constexpr static size_t maxTriangles = 128;
        constexpr static size_t maxQuads = 128;
        size_t triangle_count = 0;
        size_t quad_count = 0;
        std::array<TriangleVertex, maxTriangles> triangle_vertices;
        std::array<TriangleVertex, maxQuads> quad_vertices;
        unsigned int VBO_triangles; // GPU side handle.
        unsigned int VBO_quads;     // GPU side handle.

        unsigned int VAO_Triangles; // Recipe
        unsigned int VAO_Quads; // Recipe

        bool d_wireframe = false;
    };
} // namespace Nilib

#endif