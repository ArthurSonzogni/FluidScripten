#include "FluidApplication.hpp"
#include <iostream>

using namespace std;

const string vertex = R"(

attribute vec4 vPosition;   
varying vec2 fPos;
void main()                 
{
    fPos = vPosition.xy;
    float x = vPosition.x;
    float y = vPosition.y;
    gl_Position = vec4(2.0*x-1.0,2.0*y-1.0,0.0,1.0);
}                           

)";
const string fragment =  
#ifdef EMSCRIPTEN
"precision mediump float;                   \n"
#endif
R"(
varying vec2 fPos;
uniform sampler2D intensity;
void main()                                
{                                          
    float d = texture2D(intensity,fPos).r;
    gl_FragColor = vec4(d,d,d,1.0);
}                                          
)";

void FluidApplication::init()
{
    intensity.resize(width*height,128);

    // load shader
    program.reset(new ShaderProgram(vertex,fragment));

    // load rectangle data
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat vVertices[] =
    {
        0.0f , 0.0f , 0.0f ,
        1.0f , 0.0f , 0.0f ,
        1.0f , 1.0f , 0.0f ,

        0.0f , 0.0f , 0.0f ,
        1.0f , 1.0f , 0.0f ,
        0.0f , 1.0f , 0.0f ,
    };
    glBufferData(GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), &vVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // load texture
    buildTexture();
}

void FluidApplication::step()
{
    updateTexture();

    glUseProgram(program->data());

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(glGetUniformLocation(program->data(),"intensity"),0);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3*2); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    static int i = 0;
    ++i;
    for(int64_t y = 0; y<height; ++y)
    {
        intensity[width/2 + y * width] = (i&64) ? 0 : 255;
        intensity[y + y * width] = (i&64) ? 0 : 255;
        intensity[y + (height-y) * width] = (i&64) ? 0 : 255;
    }



    for(int64_t y = 0; y<height; ++y)
    {
        for(int64_t x = 0; x<width; ++x)
        {
            int64_t i = x + width * y;
            int64_t j = (x-1+width)%width + width*y;
            auto& I = intensity[i];
            auto& J = intensity[j];
            auto mean = (I + J)/2;
            int64_t diff = (I - mean) * 1.1;
            I -= diff;
            J += diff;
        }
        for(int64_t x = 0; x<width; ++x)
        {
            int64_t i = width-x + width * y;
            int64_t j = (width-x-1+width)%width + width*y;
            auto& I = intensity[i];
            auto& J = intensity[j];
            auto diff = (J - I) * 0.1;
            I -= diff;
            J += diff;
        }
    }
}

void FluidApplication::buildTexture()
{
    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D,texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, (const GLvoid*)intensity.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);
}

void FluidApplication::updateTexture()
{
    glBindTexture(GL_TEXTURE_2D,texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height ,GL_LUMINANCE, GL_UNSIGNED_BYTE, (const GLvoid*)intensity.data());
    glBindTexture(GL_TEXTURE_2D,0);
}
