#ifndef FLUIDAPPLICATION_H
#define FLUIDAPPLICATION_H

#include "application/Application.hpp"
#include "graphics/ShaderProgram.hpp"
#include <memory>
#include <vector>

class FluidApplication : public Application
{
    public: 
        virtual void init() override;
        virtual void step() override;
    private:
        std::unique_ptr<ShaderProgram> program;
        GLuint vbo = 0;
        GLuint texture_id = 0;
        GLint  texture_uniform_id = 0;

        std::vector<uint8_t> intensity;
        void buildTexture();
        void updateTexture();
        int width = 512;
        int height = 512;
};

#endif /* end of include guard: FLUIDAPPLICATION_H */
