#include "Nilib/Renderer/Renderer.h"

#include "GLAD/glad.h"

using namespace Nilib;

Renderer::Renderer()
{
    // Enable COLOR blending.     
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable Anti-aliasing
    glEnable(GL_MULTISAMPLE);  

    glGenVertexArrays(1, &VAO_triangles);
    glGenBuffers(1, &VBO_triangles);

    // Set up the VAO and VBO.
    glBindVertexArray(VAO_triangles);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_triangles);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->triangle_vertices), &triangle_vertices, GL_STATIC_DRAW);
    
    // Attribute position.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer::TriangleVertex), (void*)0);
    glEnableVertexAttribArray(0);
    // Attribute color.
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Renderer::TriangleVertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Renderer::drawCalls() 
{
    // DrawCall
    glBindVertexArray(VAO_triangles);
    glDrawArrays(GL_TRIANGLES, 0, triangle_count);
}

// Bind triangle Buffer.
void Renderer::bindTriangles() const
{

}



//void drawSimpleTriangle(Vec3f position, Vec4f color);
void Renderer::submitTriangle(TriangleVertex const &v1, TriangleVertex const &v2, TriangleVertex const &v3)
{
    if (triangle_count > maxTriangles - 3) { 
        LOG_WARNING("Not enough space for triangle on Renderer!");
        return;
    }
    LOG_DEBUG("Pushing triangle vertex", v1.position, v1.color);
    v1.position.print(); v1.color.print();
    triangle_vertices[triangle_count++] = v1;
    triangle_vertices[triangle_count++] = v2;
    triangle_vertices[triangle_count++] = v3;
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->triangle_vertices), &triangle_vertices, GL_STATIC_DRAW);
    
}


Renderer::~Renderer()
{
    // Cleanup
    glDeleteVertexArrays(1, &VAO_triangles);
    glDeleteBuffers(1, &VBO_triangles);
}
