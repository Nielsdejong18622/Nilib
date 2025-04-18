#include "GLAD/glad.h"

#include "Nilib/Renderer/Renderer.h"

using namespace Nilib;

Renderer::Renderer()
{
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("GLAD initialization failed!");
        throw std::exception();
    }

    // Enable COLOR blending.     
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable Anti-aliasing
    glEnable(GL_MULTISAMPLE);  



    // Set up the VAO and VBO.
    glGenVertexArrays(1, &VAO_triangles);
    glGenBuffers(1, &VBO_triangles);

    glBindVertexArray(VAO_triangles);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_triangles);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), &triangle_vertices, GL_STATIC_DRAW);
    
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
void Renderer::endScene() const
{

}

void Renderer::startScene(Window const &window) const
{
    
}



//void drawSimpleTriangle(Vec3f position, Vec4f color);
void Renderer::submitTriangle(TriangleVertex const &v1, TriangleVertex const &v2, TriangleVertex const &v3)
{
    if (triangle_count > maxTriangles - 3) { 
        LOG_WARNING("Not enough space for triangle on Renderer!");
        return;
    }
    // For bounds. 
    submitted_x.push(v1.position.x());
    submitted_x.push(v2.position.x());
    submitted_x.push(v3.position.x());

    submitted_y.push(v1.position.y());
    submitted_y.push(v2.position.y());
    submitted_y.push(v3.position.y());
    
    submitted_z.push(v1.position.z());
    submitted_z.push(v2.position.z());
    submitted_z.push(v3.position.z());

    LOG_DEBUG("Pushing triangle vertex", v1.position, v1.color);
    v1.position.print(); v1.color.print();
    triangle_vertices[triangle_count++] = v1;
    triangle_vertices[triangle_count++] = v2;
    triangle_vertices[triangle_count++] = v3; 

    // TODO: Perhaps use subbufferdata?
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->triangle_vertices), &triangle_vertices, GL_STATIC_DRAW);

}


Renderer::~Renderer()
{
    // Cleanup
    glDeleteVertexArrays(1, &VAO_triangles);
    glDeleteBuffers(1, &VBO_triangles);
}
