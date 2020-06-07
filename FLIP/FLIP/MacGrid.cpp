#include <MacGrid.h>
#include <Cell.h>
#include <iostream>
using namespace std;

void MacGrid::update(Particle* particles, int count) {
	//set layer of all cells to -1
	for (auto i = htmap.begin(); i != htmap.end(); i++) {
	
		i->second.layer = -1;
	}

	//update cells that currently have fluid in them
	for (unsigned int i = 0; i < count; i++) {
		if (htmap.find(getHashFunc(particles[i].x, particles[i].y, particles[i].z)) == htmap.end()) { // if cell does not exist
			if (int(particles[i].x / _DELTA_X) < _SIZE_X && int(particles[i].y / _DELTA_Y) < _SIZE_Y && int(particles[i].z / _DELTA_Z) < _SIZE_Z && particles[i].x >= 0 && particles[i].y >= 0 && particles[i].z >=0) { // if p or c within sim bounds // THIS IS WRONG
				
				htmap[getHashFunc(particles[i].x, particles[i].y, particles[i].z)] = Cell(FLUID, 0, int(particles[i].x / _DELTA_X), int(particles[i].y / _DELTA_Y), int(particles[i].z / _DELTA_Z));
			}
		}
		else { //should check for if object solid but has not current concept of solids
			Cell* c = &htmap[getHashFunc(particles[i].x, particles[i].y, particles[i].z)];
			c->cellType = FLUID;
			c->layer = 0;
		}
	}

	//create buffer zone around fluid
	for (unsigned int i = 1; i < 2; i++) { //2 is user defined constant
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
	
		if (i->second.layer == -1) {
			htmap.erase(i++);
		}
		else {
			++i;
		}
		
	}

}

void MacGrid::updateBuffer(int x, int y, int z ,int i) {
	if (htmap.find(getHashFunc(x,y,z)) != htmap.end()) { // if n exisits
		Cell* c = &htmap[getHashFunc(x, y,z)];
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

bool MacGrid::cellCheck(int x, int y, int z) {
	return htmap.find(getHashFunc(x, y, z)) != htmap.end();
}
