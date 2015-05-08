#include "Application.hpp"
#include <iostream>

#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

using namespace std;


Application* application = nullptr;
static void main_loop()
{
    application->step();
}


void Application::exec()
{
    application = this;
    SDL_SetMainReady(); 
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        cerr << "SDL_Init failed!\n";
        cerr << SDL_GetError() << endl;
        throw 0;
    }
    width = 800;
    height = 600;
    loadOpenGLContext();

    this->init();
#ifdef EMSCRIPTEN
    emscripten_set_main_loop(main_loop, 30, true);
#else
    while (!done) {
        em_arg_callback_func();
    }
#endif

    SDL_Quit();

}

void Application::loadOpenGLContext()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0); 

    window = SDL_CreateWindow("Fluid_Emscripten", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == nullptr)
    {
        cerr << "Error when creating SDL GL Window: '" << SDL_GetError() << "'\n";
    }

    context = SDL_GL_CreateContext(window);
    if (!context)
    {
        cerr << "Error when creating SDL GL Context: '" << (SDL_GetError()) <<"'\n";
    }

    SDL_GetWindowSize(window, &width, &height);
}

void Application::step()
{
    static int step_counter = 0;
    cout << "Step = " << step_counter++ << endl;
}

void Application::init()
{
}
