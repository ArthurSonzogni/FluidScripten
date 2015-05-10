template<int N>
void Simulation<N>::set_boundary(D& data, float value)
{
    for (int x=0; x<N; x++) data[P(x,0  )] = value;
    for (int x=0; x<N; x++) data[P(x,N-1)] = value;
    for (int y=0; y<N; y++) data[P(0  ,y)] = value;
    for (int y=0; y<N; y++) data[P(N-1,y)] = value;
}

template<int N>
void Simulation<N>::diffuse(const D& input, D& output, float dt)
{
    float a = dt;
    float A = a / ( 1.0f + 4.0f * a);
    for (int k=0 ; k<20 ; k++ )
    {
        for (int y=1 ; y<N-1 ; y++ )
        for (int x=1 ; x<N-1 ; x++ )
        { 
            output[P(x,y)] = (
                                    input[P(x,y)] + A *
                                    (
                                        output[P(x-1 , y   )] +
                                        output[P(x+1 , y   )] +
                                        output[P(x   , y-1 )] +
                                        output[P(x   , y+1 )]
                                    )
                             );
        }
        set_boundary( output, 0.f);
    }
}

template<int N>
void Simulation<N>::advect(const D& input, D& output, const D& dx, const D& dy, float dt)
{
    for (int y=1 ; y<N-1 ; y++ )
    for (int x=1 ; x<N-1 ; x++ )
    { 
        // find source of density
        float X = x - dt * dx[P(x,y)]; 
        float Y = y - dt * dy[P(x,y)]; 

        // clamp on region
        if (X<0.5f) X = 0.5f;
        if (Y<0.5f) Y = 0.5f;
        if (X>N-1.5f) X = N-1.5f;
        if (Y>N-1.5f) Y = N-1.5f;

        // linear interpolation
        const int X0 = X;
        const int Y0 = Y;
        const int X1 = X0+1; 
        const int Y1 = Y0+1; 
        const float lx = X-X0;
        const float ly = Y-Y0;

        output[P(x,y)] = lx * ( ly * input[P(X0,Y0)] + (1.0f - ly) * input[P(X0,Y0)] ) +
                   (1.0f-lx) * ( ly * input[P(X0,Y0)] + (1.0f - ly) * input[P(X0,Y0)] ) ;
    }
} 

template<int N>
void Simulation<N>::add_source(const D& source, D& output, float dt)
{
    for(int i = 0; i<N*N; ++i)
        output[i] += source[i] * dt;
}


template<int N>
void Simulation<N>::evolve_density(float dt)
{
    add_source(source, density, dt);
    diffuse(density,buffer,dt*diffusion);
    advect(buffer,density,velocityX,velocityY,dt);
}
