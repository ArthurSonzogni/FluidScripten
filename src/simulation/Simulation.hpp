#ifndef SIMULATION_H
#define SIMULATION_H

//#include <array>
#include <vector>

template<int N>
class Simulation
{
    public:
        typedef std::vector<float> D;
        // the data
        D source    = D(N*N,0.f);
        D density   = D(N*N,0.f);
        D velocityX = D(N*N,0.f);
        D velocityY = D(N*N,0.f);
        float dt = 1.0f;
        float diffusion = 1.0f;
    private:
        void evolve_density(float dt);
        // internal data
        D buffer = D(N*N,0.0f);
    private:
        // utilities
        static inline int P(int x, int y) { return x + N * y ;}
        static void set_boundary(D& data, float value);
        static void diffuse(const D& input, D& output, float dt);
        static void advect(const D& input, D& output, const D& dx, const D& dy, float dt);
        static void add_source(const D& source, D& output, float dt);

};

#include "Simulation.ipp"

#endif /* end of include guard: SIMULATION_H */
