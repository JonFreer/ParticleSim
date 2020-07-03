#pragma once
#include <array2d.h>
#include <markerparticle.h>
class FluidSolver {
public:
    FluidSolver(int size_x, int size_y, float dx, int particleCount) :SIZE_X(size_x), 
        SIZE_Y(size_y),
        DX(dx),
        particleCount(particleCount),
        pressure(Array2d<float>(size_x,size_y,0.0f)), 
        u(Array2d<float>(size_x+1, size_y, 0.0f)),
        v(Array2d<float>(size_x, size_y+1, 0.0f)),
        temp_u(Array2d<float>(size_x + 1, size_y, 0.0f)),
        temp_v(Array2d<float>(size_x, size_y+1, 0.0f)),
        materialGrid(Array2d<int>(size_x, size_y, M_AIR)),
        layerGrid(Array2d<int>(size_x, size_y,-1)){};

    void run(int frames, Particle* data, int* gridData);


    Array2d<int> materialGrid;
    int SIZE_X;
    int SIZE_Y;
    float DX;
    int particleCount;

private:

    float _getTimeStep();
    void _initializeSimulation();
    void _initializeFluidCells();
    void _resetExtrapolatedVelocity();
    bool _borderFluidCellU(int x, int y);
    bool _borderFluidCellV(int x, int y);
    void _copyData(int frame, Particle* data, int* gridData);
    Array2d<float> pressure;
    Array2d<float> u; // velocity in the x component
    Array2d<float> v; // velocity in the y component
    Array2d<float> temp_u; 
    Array2d<float> temp_v;
    Array2d<int> layerGrid;

 

    int M_AIR = 0;
    int M_FLUID = 1;
    int M_SOLID = 2;

    Particle* particles;

    
};