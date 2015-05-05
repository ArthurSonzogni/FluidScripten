#include "SDL.h"
#include <GLES2/gl2.h>
#include <emscripten.h>
#include <iostream>


void process()
{
    // as we don't have a timer we need to do something here
    // using a static to update at an interval
    static int t=0;
    if(++t > 100)
    {
        float r=(double)rand() / ((double)RAND_MAX + 1);
        float g=(double)rand() / ((double)RAND_MAX + 1);
        float b=(double)rand() / ((double)RAND_MAX + 1);

        glClearColor(r,g,b,1);
        t=0;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    // this is where we draw
    SDL_GL_SwapBuffers();
}

int main(int argc, char *argv[])
{
    SDL_Surface *screen;

    // Init SDL
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) 
    {
        std::cerr<<"Unable to initialize SDL: "<<SDL_GetError();
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    screen = SDL_SetVideoMode( 720, 576, 16, SDL_OPENGL  | SDL_RESIZABLE); 
    if ( !screen ) 
    {
        std::cerr<<"Unable to set video mode: "<<SDL_GetError();
        return EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST);

    // let emscripten process something then 
    // give control back to the browser
    emscripten_set_main_loop (process, 0, true);

    SDL_Quit();
    return EXIT_SUCCESS;
}
