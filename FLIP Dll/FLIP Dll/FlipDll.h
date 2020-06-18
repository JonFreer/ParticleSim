#pragma once
#include <Particle.h>
#include <Cell.h>
#ifdef SIMDLL_EXPORTS
#define SIM_API __declspec(dllexport)
#else
#define SIM_API __declspec(dllimport)
#endif

//Initilise the simulation
extern "C" SIM_API void sim_init(int count,int frames,Particle **out, Cell **grid, int** cellCount);



extern "C" SIM_API void sim_free();