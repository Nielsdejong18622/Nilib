#include "GLAD/glad.h"

#include "Nilib/Renderer/Renderer.h"

using namespace Nilib;

Renderer::Renderer()
    : triangle_shader(ShaderProgram::createFromFiles("shaders/vertex.vert", "shaders/fragment.frag")),
      quad_shader(ShaderProgram::createFromFiles("shaders/quad.vert", "shaders/quad.frag"))
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

    glGenVertexArrays(1, &VAO_Triangles); // Record triangle VAO.
    glBindVertexArray(VAO_Triangles);

    glGenBuffers(1, &VBO_triangles);
    glGenVertexArrays(1, &VAO_Quads); // Record quad VAO.

    glBindBuffer(GL_ARRAY_BUFFER, VBO_triangles);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), &triangle_vertices, GL_STATIC_DRAW);

    // Set vertex attribs for triangles
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer::TriangleVertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Renderer::TriangleVertex), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Done with triangle VAO

    // Setup VAO and VBO for quads
    glBindVertexArray(VAO_Quads);

    glGenBuffers(1, &VBO_quads);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_quads);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);

    // Set vertex attribs for quads
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer::TriangleVertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Renderer::TriangleVertex), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Done with quad VAO
}

void Renderer::toggleWireFrameMode()
{
    if (d_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    d_wireframe = !d_wireframe;
}

void Renderer::drawCalls()
{
    // LOG_DEBUG("Drawing:", triangle_count, quad_count);

    // Draw triangles
    triangle_shader.bind();
    glBindVertexArray(VAO_Triangles);
    glDrawArrays(GL_TRIANGLES, 0, triangle_count);

    // Draw quads
    quad_shader.bind();
    // Set the shader uniforms.
    int cameraLocation = glGetUniformLocation(quad_shader.id, "camera");
    ASSERT(cameraLocation != -1, "Camera could not be found in the quad_shader!");
    glUniform3f(cameraLocation, camera.position.x(), camera.position.y(), camera.position.z());

    glBindVertexArray(VAO_Quads);
    glDrawArrays(GL_TRIANGLES, 0, quad_count);

    triangle_shader.bind();
}

// Bind triangle Buffer.
void Renderer::endScene() const
{
}

void Renderer::startScene(Window const &window) const
{
}

// Counter Clockwise.
void Renderer::submitQuad(TriangleVertex const &v1, TriangleVertex const &v2, TriangleVertex const &v3, TriangleVertex const &v4)
{
    if (quad_count > maxQuads - 6)
    {
        LOG_WARNING("Not enough space for triangle on Renderer!");
        return;
    }
    quad_vertices[quad_count++ % maxQuads] = v1;
    quad_vertices[quad_count++ % maxQuads] = v2;
    quad_vertices[quad_count++ % maxQuads] = v3;
    quad_vertices[quad_count++ % maxQuads] = v4;
    quad_vertices[quad_count++ % maxQuads] = v1;
    quad_vertices[quad_count++ % maxQuads] = v3;

    // TODO: Perhaps use subbufferdata?
    glBindBuffer(GL_ARRAY_BUFFER, VBO_quads);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->quad_vertices), &quad_vertices, GL_STATIC_DRAW);
}

// Counter Clockwise.
void Renderer::submitTriangle(TriangleVertex const &v1, TriangleVertex const &v2, TriangleVertex const &v3)
{
    if (triangle_count > maxTriangles - 3)
    {
        LOG_WARNING("Not enough space for triangle on Renderer!");
        return;
    }

    triangle_vertices[triangle_count++ % maxTriangles] = v1;
    triangle_vertices[triangle_count++ % maxTriangles] = v2;
    triangle_vertices[triangle_count++ % maxTriangles] = v3;

    glBindBuffer(GL_ARRAY_BUFFER, VBO_triangles);
    // TODO: Perhaps use subbufferdata?
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->triangle_vertices), &triangle_vertices, GL_STATIC_DRAW);
}

Renderer::~Renderer()
{
    // Cleanup
    glDeleteVertexArrays(1, &VAO_Triangles);
    glDeleteBuffers(1, &VBO_triangles);

    glDeleteVertexArrays(1, &VAO_Quads);
    glDeleteBuffers(1, &VBO_quads);
}
