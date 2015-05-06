#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <string>

class ShaderProgram
{
    public:
        ShaderProgram(const std::string& vertex, const std::string& fragment);
        ~ShaderProgram();
    private:
        GLuint loadShader(GLenum type, const std::string& source);
        GLuint vert;
        GLuint frag;
        GLuint id;
};

#endif /* end of include guard: SHADERPROGRAM_H */
