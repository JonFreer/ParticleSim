#include <MacGrid.h>
#include <Cell.h>
#include <iostream>
#include <assert.h>
using namespace std;

void MacGrid::update(Particle* particles, int count) {
	//set layer of all cells to -1
	for (auto i = htmap.begin(); i != htmap.end(); i++) {
		i->second.layer = -1;
	}

	//update cells that currently have fluid in them
	for (unsigned int i = 0; i < count; i++) {
		if (int(particles[i].x / _DELTA_X) < _SIZE_X && int(particles[i].y / _DELTA_X) < _SIZE_Y && int(particles[i].z / _DELTA_X) < _SIZE_Z && particles[i].x >= 0 && particles[i].y >= 0 && particles[i].z >= 0) {
			if (htmap.find(getHashFuncParticles(particles[i].x, particles[i].y, particles[i].z)) == htmap.end()) { // if cell does not exist
				// if p or c within sim bounds // THIS IS WRONG
				assert(particles[i].z > 0);
				htmap[getHashFuncParticles(particles[i].x, particles[i].y, particles[i].z)] = Cell(FLUID, 0, int(particles[i].x / _DELTA_X), int(particles[i].y / _DELTA_X), int(particles[i].z / _DELTA_X));

			}
			else { //should check for if object solid but has not current concept of solids

				Cell* c = getCell(getHashFuncParticles(particles[i].x, particles[i].y, particles[i].z));
				if (c->cellType != SOLID) { //cant overwrite solids
					assert(c->x == int(particles[i].x / _DELTA_X) && c->y == int(particles[i].y / _DELTA_X) && c->z == int(particles[i].z / _DELTA_X));
					c->cellType = FLUID;
					c->layer = 0;
				}

			}
		}
	}

	//create buffer zone around fluid


	for (unsigned int i = 1; i < 3; i++) { //2 is user defined constant
		for (std::pair<int, Cell> element : htmap) { //for each cell
			if ((element.second.cellType == AIR || element.second.cellType == FLUID) && element.second.layer == i - 1) {
				updateBuffer(element.second.x - 1, element.second.y, element.second.z, i);
				updateBuffer(element.second.x + 1, element.second.y, element.second.z, i);
				updateBuffer(element.second.x, element.second.y + 1, element.second.z, i);
				updateBuffer(element.second.x, element.second.y - 1, element.second.z, i);
				updateBuffer(element.second.x, element.second.y, element.second.z + 1, i);
				updateBuffer(element.second.x, element.second.y, element.second.z-1, i);
			}
		}
	}
	
	for (auto i = htmap.begin(); i != htmap.end();) {
		i->second.density = 0;
		if (i->second.layer == -1) {
			htmap.erase(i++);
		}
		else {
			++i;
		}
		
	}

}

void MacGrid::validCells() {
	for (auto i = htmap.begin(); i !=htmap.end(); i++) {
		assert(i->second.x > - 5);
	}
}

void MacGrid::updateBuffer(int x, int y, int z ,int i) {
	if (htmap.find(getHashFunc(x,y,z)) != htmap.end()) { // if n exisits
		Cell* c = getCell(x, y,z);
		assert(c->x == x && c->y == y && c->z == z);
		if (c->layer == -1){
			if (c->cellType != SOLID) {
				c->cellType = AIR;
				c->layer = i;
			}
			else {
				c->layer = i;
			}
		}
	}
	else {
		if (x < _SIZE_X && x >= 0 && y < _SIZE_Y && y >= 0 && z < _SIZE_Z && z >= 0) { // if in bounds
			
			htmap[getHashFunc(x, y, z)] = Cell(AIR, i, x, y, z);
		}
		else {
			
			htmap[getHashFunc(x, y, z)] = Cell(SOLID, i, x, y, z);
		}
	}
}

void MacGrid::print() {
	for (std::pair<int, Cell> element : htmap) {

		cout << " X: " << element.second.x << " Y: " << element.second.y << " Z: " << element.second.z << " TYPE: " << element.second.cellType<< "\n VEL X: " << element.second.Velocity.x << " VEL Y: " << element.second.Velocity.y << " VEL Z: " << element.second.Velocity.z << "\n";
	}
}

Cell* MacGrid::getCell(int x, int y, int z) {
	return &htmap[getHashFunc(x, y, z)];
}

Cell* MacGrid::getCell(int f) {
	return &htmap[f];
}

bool MacGrid::cellCheck(int f) {
	return htmap.find(f) != htmap.end();
}


bool MacGrid::cellCheck(int x, int y, int z) {
	return htmap.find(getHashFunc(x, y, z)) != htmap.end();
}
