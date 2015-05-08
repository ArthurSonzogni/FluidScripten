#include "FluidApplication.hpp"
#include <iostream>

using namespace std;

const string vertex = R"(

attribute vec4 vPosition;   
void main()                 
{
   gl_Position = vPosition; 
}                           

)";
const string fragment =  
#ifdef EMSCRIPTEN
  "precision mediump float;                   \n"
#endif
  "void main()                                \n"
  "{                                          \n"
  "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
  "}                                          \n";

void FluidApplication::init()
{
    program.reset(new ShaderProgram(vertex,fragment));
    cout << flush;
}
