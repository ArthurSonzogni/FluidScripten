template<int N>
Simulation<N>::Simulation()
{
}
    
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
    for (int k=0 ; k<20 ; k++ )
    {
        for (int y=1 ; y<N-1 ; y++ )
        for (int x=1 ; x<N-1 ; x++ )
        { 
            output[P(x,y)] = (
                                    input[P(x,y)] + a * 
                                    (
                                        output[P(x-1 , y   )] +
                                        output[P(x+1 , y   )] +
                                        output[P(x   , y-1 )] +
                                        output[P(x   , y+1 )]
                                    )
                             ) / ( 1.0f + 4.0f * a );
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

        output[P(x,y)] = (1.0-lx) *  ( (1.0f-ly) * input[P(X0,Y0)] + ly * input[P(X0,Y1)] ) +
                              lx   * ( (1.0f-ly) * input[P(X1,Y0)] + ly * input[P(X1,Y1)] ) ;
    }
} 



template<int N>
void Simulation<N>::evolve_density()
{
    diffuse(density,buffer_1,dt*diffusion);
    advect(buffer_1,density,velocityX,velocityY,dt);
}

template<int N>
void Simulation<N>::evolve_velocity()
{
    diffuse(velocityX,buffer_1,dt*viscosity);
    diffuse(velocityY,buffer_2,dt*viscosity);

    project(buffer_1,buffer_2,velocityX,velocityY);

    advect(buffer_1, velocityX, buffer_1, buffer_2, dt);
    advect(buffer_2, velocityY, buffer_1, buffer_2, dt);

    project(velocityX, velocityY, buffer_1,buffer_2);
}

template<int N>
void Simulation<N>::project(D& dx, D& dy, D& pressure, D& divergence)
{
    // compute divergence
    for (int y=1 ; y<N-1 ; y++ )
    for (int x=1 ; x<N-1 ; x++ )
    {
        divergence[P(x,y)] = - 0.5f *
        (
                dx[P(x+1 , y  )] - dx[P(x-1 , y  )] +
                dy[P(x   , y+1)] - dy[P(x   , y-1)]
        );
    }
    set_boundary(divergence,0.0f);

    // compute pressure
    for(int i = 0; i<N*N; ++i) pressure[i] = 0.0f;
    for (int k=0 ; k<20 ; k++ )
    {
        for (int y=1 ; y<N-1 ; y++ )
        for (int x=1 ; x<N-1 ; x++ )
        { 
            pressure[P(x,y)] = 0.25f * (
              divergence[P(x  ,y  )] + 
                pressure[P(x-1,y  )] +
                pressure[P(x+1,y  )] +
                pressure[P(x  ,y-1)] +
                pressure[P(x  ,y+1)] );
        }
    }

    // compute projected field
    for (int y=1 ; y<N-1 ; y++ )
    for (int x=1 ; x<N-1 ; x++ )
        dx[P(x,y)] -= 0.5f * ( pressure[P(x+1,y  )] - pressure[P(x-1,y  )] );

    for (int y=1 ; y<N-1 ; y++ )
    for (int x=1 ; x<N-1 ; x++ )
        dy[P(x,y)] -= 0.5f * ( pressure[P(x  ,y+1)] - pressure[P(x  ,y-1)] );
}

template<int N>
void Simulation<N>::evolve()
{
    evolve_density();
    evolve_velocity();
}
