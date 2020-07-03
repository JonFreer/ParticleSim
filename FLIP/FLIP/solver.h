#pragma once
#include <MacGrid.h>
#include <Particle.h>

class Solver {

public:
	Solver(int count_, int size_x, int size_y, int size_z, float cell_size);

	MacGrid grid;
	Particle* particles;
	unsigned int count;

	void run(float delta_t, int frames, Particle* data, Cell* gridData, int* cellCount);
	void copyData(int frame, Particle* data, Cell* gridData, int* cellCount);
	float getInterpolatedValueX(float x, float y, float z);
	float getInterpolatedValueY(float x, float y, float z);
	float getInterpolatedValueZ(float x, float y, float z);
	Vector getVelocity(float x, float y, float z);
	Point traceParticle(float x, float y, float z, float t);
	void backTrace(float delta_t);
	void forwardTrace(float delta_t);
	void applyViscoity(float viscosity, float delta_t);
	void applyGravity(float delta_t);
	void pressureSolve(float delta_t);
	void extrapolateVelocitiesHelper(int x, int y, int z, float* sumVelocityX, float* sumVelocityY, float* sumVelocityZ, int* neighbour_count, int i);
	void extrapolateVelocities();
	void checkForSolid();
	void outputParticles(); //print

private:
	void _resetExtraoplatedFluidVelocities();
	bool _checkBorderFLUIDX(int x, int y, int z);
	bool _checkBorderFLUIDY(int x, int y, int z);
	bool _checkBorderFLUIDZ(int x, int y, int z);



};