#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <FlipDll.h>
#include <Solver.cpp>
#include <Particle.h>
#include <Particle.cpp>
#include <MacGrid.cpp>


Solver s;

void sim_init(int count,int frames,Particle** out, Cell** gridOut , int** cellCountOut) {
	
	s = Solver(count);
	Particle* data = (Particle*)malloc(sizeof(Particle) * count * frames);
	Cell* grid = (Cell*)malloc(sizeof(Cell) * 200 * frames);
	int* cellCount = (int*)malloc(sizeof(int) * frames);
	s.run(0.05, frames, data ,grid , cellCount);

	*gridOut = grid;
	*cellCountOut = cellCount;
	*out = data;
}


