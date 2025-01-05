#include "Nilib/Renderer/ShaderProgram.h"
#include "Nilib/Logger/Log.hpp"
#include "Nilib/Core/Assert.hpp"

#include "Glad/glad.h"
#include <GLFW/glfw3.h>

using namespace Nilib;

ShaderProgram::ShaderProgram()
    : id(glCreateProgram())
{
    if (!id)
        LOG_ERROR("Unable to create Program!");
}

ShaderProgram::ShaderProgram(int shaderid)
    : id(shaderid)
{
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id);
    id = 0;
}

// Function to compile shaders
unsigned int ShaderProgram::compileShader(unsigned int shaderType, const char *shaderSource)
{
    LOG_DEBUG("Compiling", ((shaderType == 0x8B31) ? "VertexShader" : "FragmentShader"));
    ASSERT(glfwGetCurrentContext(), "No associated GLAD context!");
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("GLAD initialization failed!");
        return 0;
    }

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        LOG_ERROR("[SHADER] COMPILATION_FAILED", infoLog);
        return 0;
    }
    CORE_ASSERT(shader);
    return shader;
}

// Function to create a shader program
ShaderProgram ShaderProgram::createFromFiles(const char *vertexsourcefilename, const char *fragmentsourcefilename)
{
    // Open the file
    std::ifstream vertexfile(vertexsourcefilename);
    std::ifstream fragmentfile(fragmentsourcefilename);

    // Check if the file is open
    if (!vertexfile.is_open())
    {
        LOG_ERROR("Error when opening file", vertexsourcefilename);
        return ShaderProgram(0);
    }
    // Check if the file is open
    if (!fragmentfile.is_open())
    {
        LOG_ERROR("Error when opening file", fragmentsourcefilename);
        return ShaderProgram(0);
    }

    // Read the entire content of the file
    std::stringstream vert_buffer, fragm_buffer;
    vert_buffer << vertexfile.rdbuf();
    fragm_buffer << fragmentfile.rdbuf();
    return createFromStrings(vert_buffer.str().c_str(), fragm_buffer.str().c_str());
}

// Function to create a shader program
ShaderProgram ShaderProgram::createFromStrings(const char *vertexSource, const char *fragmentSource)
{
    LOG_DEBUG("Creating shader!");
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // Create the program and link shaders
    unsigned int programid = glCreateProgram();
    glAttachShader(programid, vertexShader);
    glAttachShader(programid, fragmentShader);
    glLinkProgram(programid);

    // Check for linking errors
    GLint success;
    glGetProgramiv(programid, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(programid, 512, nullptr, infoLog);
        LOG_ERROR("Shader Program LINKING_FAILED", infoLog);
        return 0;
    }

    // Clean up shaders as they're no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return ShaderProgram(programid);
}

void ShaderProgram::bind() const
{
    glUseProgram(id);
}