#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

class Application
{
    public:
        void exec();
        virtual void step();
        virtual void init();
    private:
        void loadOpenGLContext();
    private: // data
        SDL_Window* window = nullptr;
        SDL_GLContext context = nullptr;
    protected: // data
        int width = 0;
        int height = 0;
};

#endif /* end of include guard: APPLICATION_H */
