#pragma once
#include <solver.h>
class FluidRenderer {

public:
	FluidRenderer(){}
	FluidRenderer(Solver *fs, Cell *cells, int *cellCount ,Particle *particleData):fs(fs),
		cells(cells),
		cellCount(cellCount),
		particleData(particleData)
	{}

	void drawBoundingBox();
	void drawCellsByType(int frame, CellType type,float r, float g, float b,int i);
	void getCenter(float *x, float *y, float *z);
	void drawParticles(int frame);
	void drawVelocitites(int frame,int i);
private:
	Solver *fs;
	Cell* cells;
	int* cellCount;
	Particle* particleData;
	void _drawWireframeCube(float x, float y, float z, float size, float r, float g, float b);
	void _drawGridCellsByType(CellType type);
	void _drawSphere(float x, float y, float z, float r);
};	