#pragma once
#include "Struct.h"


#ifdef SIMDLL_EXPORTS
#define SIM_API __declspec(dllexport)
#else
#define SIM_API __declspec(dllimport)
#endif

//Initilise the simulation
extern "C" SIM_API void sim_init(const int count, const int frames,const int temp, const float xSpan, const float ySpan, const float zSpan,XYZ **out1);

extern "C" SIM_API bool sim_check();