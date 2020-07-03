#pragma once

#include <fluidsimulation.h>
class FluidRenderer {

public:
	FluidRenderer() {}
	FluidRenderer(FluidSolver* fs , int* g, Particle* p) :fs(fs), grid(g), particles(p)
	{}

	void drawBoundingBox();
	void drawCellsByType(int frame, int type, float r, float g, float b, int i);
	void getCenter(float* x, float* y, float* z);
	void drawParticles(int frame);
	void drawVelocitites(int frame, int i);

private:
	FluidSolver* fs;
	int* grid;
	Particle* particles;
	void _drawWireframeCube(float x, float y, float z, float size, float r, float g, float b);
	void _drawGridCellsByType(int type);
	void _drawSphere(float x, float y, float z, float r);
};