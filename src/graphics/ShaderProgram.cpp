#include "ShaderProgram.hpp"
#include <iostream>

using namespace std;

ShaderProgram::ShaderProgram(const std::string& vertex, const std::string& fragment)
{
    cout << "loading vertex shader" << endl;
    vert = loadShader(GL_VERTEX_SHADER,vertex); 
    cout << "loading fragment shader" << endl;
    frag = loadShader(GL_FRAGMENT_SHADER,fragment);

    cout << vert << "," << frag << endl;
}

ShaderProgram::~ShaderProgram()
{
    if (vert) glDeleteShader(vert);
    if (frag) glDeleteShader(frag);
    if (id)   glDeleteProgram(id);
}

GLuint ShaderProgram::loadShader(GLenum type, const std::string& source)
{
    // create a new shader
    GLuint id = glCreateShader(type);
    if (id == 0) return 0;

    // associate the source
    const char* d = source.data();
    const char** dd = &d;
    glShaderSource(id, 1, dd, NULL);

    // compile the shader
    glCompileShader(id);
    GLint isCompiled = false;

    // check compilation output
    glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        GLint length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        if (length > 1)
        {
            string info(length,' '); 
        }
        glDeleteShader(id);
        return 0;
    }
    else return id;
}

