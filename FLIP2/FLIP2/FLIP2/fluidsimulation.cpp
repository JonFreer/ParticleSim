#include <fluidsimulation.h>
#include <stdlib.h>  
float FluidSolver::_getTimeStep() {
	return 0.1;
}

void FluidSolver::_initializeSimulation() {

	//init solid cells on border
	for (int i = 0; i < SIZE_X; i++) {
		materialGrid.set(i, 0, M_SOLID);
		materialGrid.set(i, SIZE_Y - 1, M_SOLID);
	}

	for (int j = 0; j < SIZE_Y; j++) {
		materialGrid.set(0, j, M_SOLID);
		materialGrid.set(SIZE_X-1, j, M_SOLID);
	}

	particles = (Particle*)malloc(sizeof(Particle) * particleCount);
	for (int p = 0; p < particleCount; p++) {
		particles[p] =  Particle(2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 6)), 2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 6)));
	}

	_initializeFluidCells();


}

void FluidSolver::_initializeFluidCells() {
	for (int p = 0; p < particleCount; p++) {
		if (particles[p].X > 0 && particles[p].X < SIZE_X * DX && particles[p].Y>0 && particles[p].Y < SIZE_Y * DX) { //if particle is in bounds
			int x, y;
			particles[p].getCell(&x, &y, DX);

			if (materialGrid.get(x, y)==M_AIR) {
				materialGrid.set(x, y, M_FLUID);
			}
		}
	}
}



void FluidSolver::_resetExtrapolatedVelocity() { //reset the velocities of the extraploated areas
	for (int i = 0; i < SIZE_X+1; i++) {
		for (int j = 0; j < SIZE_Y; j++) {
			if (!_borderFluidCellU(i, j)) {
				u.set(i, j, M_AIR);
			}
		}
	}

	for (int i = 0; i < SIZE_X ; i++) {
		for (int j = 0; j < SIZE_Y+1; j++) {
			if (!_borderFluidCellV(i, j)) {
				u.set(i, j, M_AIR);
			}
		}
	}

}

bool FluidSolver::_borderFluidCellU(int x, int y) {
	if (x == 0) {
		return materialGrid.get(x, y) == M_FLUID;
	}
	if (x == SIZE_X) {
		return materialGrid.get(x-1, y) == M_FLUID;
	}
	if (materialGrid.get(x, y) == 1 || materialGrid.get(x - 1, y) == 1) {
		return true;
	}
}

bool FluidSolver::_borderFluidCellV(int x, int y) {
	if (y == 0) {
		return materialGrid.get(x, y) == M_FLUID;
	}
	if (y == SIZE_Y) {
		return materialGrid.get(x , y-1) == M_FLUID;
	}
	if (materialGrid.get(x, y) == 1 || materialGrid.get(x , y-1) == 1) {
		return true;
	}
}


void FluidSolver::run(int frames, Particle* data, int* gridData) {
	_initializeSimulation();
	for (int f = 0; f < frames; f++) {
		_copyData(f, data, gridData);
	}
}

void FluidSolver::_copyData(int frame, Particle* data, int* gridData) {
	for (unsigned int i = 0; i < particleCount; i++) {
		data[i + particleCount * frame] = particles[i];
	}
	for (int j = 0; j < SIZE_X * SIZE_Y; j++) {
		gridData[j + frame * SIZE_X * SIZE_Y] = materialGrid.get(j);
	}
}