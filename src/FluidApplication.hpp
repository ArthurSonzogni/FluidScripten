#ifndef FLUIDAPPLICATION_H
#define FLUIDAPPLICATION_H

#include "application/Application.hpp"
#include "graphics/ShaderProgram.hpp"
#include <memory>
#include <vector>
#include "simulation/Simulation.hpp"

class FluidApplication : public Application
{
    public: 
        virtual void init() override;
        virtual void step() override;
    private:
        std::unique_ptr<ShaderProgram> program;
        std::unique_ptr<ShaderProgram> programWall;
        GLuint vbo = 0;
        GLuint texture_id = 0;
        GLuint texture_id_wall = 0;
        GLint  texture_uniform_id = 0;

        static const int N = 128;
        Simulation<N> simulation;
        std::vector<uint8_t> intensity = std::vector<uint8_t>(N*N,128);
        void buildTexture();
        void buildTextureWall();
        void updateTexture();
        void updateTextureWall();

        void simulate();
};

#endif /* end of include guard: FLUIDAPPLICATION_H */
