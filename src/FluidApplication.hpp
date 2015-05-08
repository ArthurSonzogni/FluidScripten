#ifndef FLUIDAPPLICATION_H
#define FLUIDAPPLICATION_H

#include "application/Application.hpp"
#include "graphics/ShaderProgram.hpp"
#include <memory>

class FluidApplication : public Application
{
    public: 
        virtual void init() override;

    private:
        std::unique_ptr<ShaderProgram> program;
};

#endif /* end of include guard: FLUIDAPPLICATION_H */
