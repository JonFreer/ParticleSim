#ifndef MAC_GRID_H
#define MAC_GRID_H



#include <unordered_map>
#include <Cell.h>
#include <Particle.h>
class MacGrid 
{
public:
	MacGrid(int size_x, int size_y, int size_z, float delta_x, float delta_y, float delta_z) :
		_SIZE_X(size_x),
		_SIZE_Y(size_y),
		_SIZE_Z(size_z),
		_DELTA_X(delta_x),
		_DELTA_Y(delta_y) ,
		_DELTA_Z(delta_z) {
		
	};
	MacGrid() {};

	std::unordered_map<int, Cell> htmap;
	
	void update(Particle* particles, int count);
	void updateBuffer(int x, int y, int z , int i);
	void print();
	// Constants
	
	int _SIZE_X;
	int _SIZE_Y;
	int _SIZE_Z;
	float _DELTA_X;
	float _DELTA_Y;
	float _DELTA_Z;
	int getHashFunc(int x, int y, int z) {
		return 541 * int(x / _DELTA_X) + 79 * int(y / _DELTA_Y) + 31 * int(z / _DELTA_Z);
	};

	Cell* getCell(int x, int y, int z);
	bool cellCheck(int x, int y, int z);

};

#endif