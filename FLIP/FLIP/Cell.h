#pragma once
struct Point {
	float x;
	float y;
	float z;
};

struct Vector {
	float x;
	float y;
	float z;
};



enum CellType { FLUID, AIR, SOLID };
class Cell {
	

public:
	
	Cell(CellType celltype , int layer ,int x,int y, int z) :
	cellType(celltype),
	layer(layer),
	x(x),
	y(y),
	z(z){
		Velocity.x = 0;
		Velocity.y = 0;
		Velocity.z = 0;
	}

	Cell() {};

	CellType cellType;
	int layer;
	int x;
	int y; 
	int z;
	Vector Velocity;
	Vector TempVelocity;
	int index;

	
};