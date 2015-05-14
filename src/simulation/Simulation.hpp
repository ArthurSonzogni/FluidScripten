#ifndef SIMULATION_H
#define SIMULATION_H

//#include <array>
#include <vector>

template<int N>
class Simulation
{
    public:
        Simulation();
        typedef std::vector<float> D;
        typedef std::vector<bool> B;
        // the data
        D density   = D(N*N,0.f);
        D velocityX = D(N*N,0.f);
        D velocityY = D(N*N,0.f);
        // the parameters
        float dt = 1.0f;
        float diffusion = 0.01f;
        float viscosity = 0.01f;
        // the method
        void evolve();
    private:
        void evolve_density();
        void evolve_velocity();
        // internal data
        D buffer_1 = D(N*N,0.0f);
        D buffer_2 = D(N*N,0.0f);
    private:
        // utilities
        static inline int P(int x, int y) { return x + N * y ;}
        static void set_boundary(D& data, float value);
        static void diffuse(const D& input, D& output, float dt);
        static void advect(const D& input, D& output, const D& dx, const D& dy, float dt);
        static void add_source(const D& source, D& output, float dt);
               void project(D& dx, D& dy, D& pressure, D& divergence);

};

#include "Simulation.ipp"

#endif /* end of include guard: SIMULATION_H */
