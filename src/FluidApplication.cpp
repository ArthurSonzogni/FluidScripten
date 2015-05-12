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
//! FRAGMENT
vec3 rainbow(float x)
{
    /*
        Target colors
        =============
        
        L  x   color
        0  0.0 vec4(1.0, 0.0, 0.0, 1.0);
        1  0.2 vec4(1.0, 0.5, 0.0, 1.0);
        2  0.4 vec4(1.0, 1.0, 0.0, 1.0);
        3  0.6 vec4(0.0, 0.5, 0.0, 1.0);
        4  0.8 vec4(0.0, 0.0, 1.0, 1.0);
        5  1.0 vec4(0.5, 0.0, 0.5, 1.0);
    */
    
    float level = floor(x * 6.0);
    float r = float(level <= 2.0) + float(level > 4.0) * 0.5;
    float g = max(1.0 - abs(level - 2.0) * 0.5, 0.0);
    float b = (1.0 - (level - 4.0) * 0.5) * float(level >= 4.0);
    return vec3(r, g, b);
}

float colormap_red(float x) {
    if (x < 0.7) {
        return 4.0 * x - 1.5;
    } else {
        return -4.0 * x + 4.5;
    }
}

float colormap_green(float x) {
    if (x < 0.5) {
        return 4.0 * x - 0.5;
    } else {
        return -4.0 * x + 3.5;
    }
}

float colormap_blue(float x) {
    if (x < 0.3) {
       return 4.0 * x + 0.5;
    } else {
       return -4.0 * x + 2.5;
    }
}

vec4 colormap(float x) {
    float r = clamp(colormap_red(x), 0.0, 1.0);
    float g = clamp(colormap_green(x), 0.0, 1.0);
    float b = clamp(colormap_blue(x), 0.0, 1.0);
    return vec4(r, g, b, 1.0);
}

void main()                                
{                                          
    //float d = texture2D(intensity,fPos).r;
    //gl_FragColor = vec4(rainbow(d)*min(1.0,d*10.0),1.0);
    //gl_FragColor = vec4(d,d,d,1.0);
    //gl_FragColor = colormap(1.0-d);

    float dn = texture2D(intensity,fPos+vec2(0.0,0.005)).r;
    float d = texture2D(intensity,fPos).r;
    float dp = texture2D(intensity,fPos-vec2(0.0,0.005)).r;
    float D = 0.5+4.0*(dp-dn);
    gl_FragColor = vec4(D*d*8.0,D,D,1.0);
}                                          
)";

void FluidApplication::init()
{
    intensity.resize(N*N,128);

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
    // simulate 1 step
    static int i = 0;
    ++i;
    const int f = 6.0f;
    for(int dx = -f; dx<f; ++dx)
    for(int dy = -f; dy<f; ++dy)
    {
        if (dx*dx+dy*dy>f*f) continue;
        simulation.sourceVelocityX[N/2 +dx + N * dy+ N*N/2] = (f*f-dx*dx+dy*dy)*cos((i/50)*M_PI/4)*10.f;
        simulation.sourceVelocityY[N/2 +dx + N * dy+ N*N/2] = (f*f-dx*dx+dy*dy)*sin((i/50)*M_PI/4)*10.f;
        simulation.sourceDensity[N/2 +dx + N * dy+ N*N/2] = 500.0f;
    }
    for(int y = 0; y<N; ++y)
    {
        simulation.sourceVelocityX[1+y*N] = 400.0f;
        simulation.sourceVelocityY[1+y*N] = 0.0f;
        simulation.sourceDensity[1+y*N] = (y/5)%2 ? 1700.0f : 0.0f;
    }

    simulation.dt = 0.05f;
    simulation.viscosity = 1.0f;
    simulation.diffusion = 1.0f;
    simulation.evolve();

    // update denstity
    for(int i = 0; i<N*N; ++i)
    {
        double dx = simulation.velocityX[i];
        double dy = simulation.velocityY[i];
        double d = simulation.density[i];
        //intensity[i] = 10.0f*(dx*dx+dy*dy);
        intensity[i] = max(0.0,min(d*0.25,255.0));
    }

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



}

void FluidApplication::buildTexture()
{
    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D,texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, N, N, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, (const GLvoid*)intensity.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);
}

void FluidApplication::updateTexture()
{
    // send the texture to openGL
    glBindTexture(GL_TEXTURE_2D,texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, N, N ,GL_LUMINANCE, GL_UNSIGNED_BYTE, (const GLvoid*)intensity.data());
    glBindTexture(GL_TEXTURE_2D,0);
}
