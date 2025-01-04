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
    
        static ShaderProgram createFromStrings(char const* vertexSource, char const* fragmentSource);
        static ShaderProgram createFromFiles(char const* vertexfilename, char const* fragmentfilename);

        void setBool(std::string const &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
        }

        void setInt(std::string const &name, int value) const;   
        
        void setFloat(std::string const &name, float value) const;

    private:
        ShaderProgram(int shaderid);
        // Constructors.
        ShaderProgram();

        // Helper function.
        static unsigned int compileShader(GLenum shaderType, char const* shaderSource);

        // ID (for OpenGL).
        unsigned int id = 0;
    };

}

#endif