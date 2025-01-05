#ifndef _SHADER_H
#define _SHADER_H

namespace Nilib
{

    class ShaderProgram
    {
    public:
        void bind() const;

        operator bool() const { return (id == 0); }
        ~ShaderProgram();

        static ShaderProgram createFromStrings(char const *vertexSource, char const *fragmentSource);
        static ShaderProgram createFromFiles(char const *vertexfilename, char const *fragmentfilename);


    private:
        ShaderProgram(int shaderid);
        // Constructors.
        ShaderProgram();

        // Helper function.
        static unsigned int compileShader(unsigned int shaderType, char const *shaderSource);

        // ID (for OpenGL).
        unsigned int id = 0;
    };

}

#endif