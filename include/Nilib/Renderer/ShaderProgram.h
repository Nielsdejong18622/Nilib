#ifndef _SHADER_H
#define _SHADER_H

#include <GLAD/glad.h>

namespace Nilib
{

    class ShaderProgram
    {
    public:        
        void bind() const;

        operator bool() const { return (id == 0);}
        ~ShaderProgram();
    
        static ShaderProgram createFromStrings(const char* vertexSource, const char* fragmentSource);
        static ShaderProgram createFromFiles(const char* vertexfilename, const char* fragmentfilename);
    private:
        ShaderProgram(int shaderid);
        // Constructors.
        ShaderProgram();

        // Helper function.
        static unsigned int compileShader(GLenum shaderType, const char* shaderSource);

        // ID (for OpenGL).
        unsigned int id = 0;
    };

}

#endif