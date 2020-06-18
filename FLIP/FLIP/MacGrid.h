#ifndef MAC_GRID_H
#define MAC_GRID_H



#include <unordered_map>
#include <Cell.h>
#include <Particle.h>
class MacGrid 
{
public:
	MacGrid(int size_x, int size_y, int size_z, float delta_x) :
		_SIZE_X(size_x),
		_SIZE_Y(size_y),
		_SIZE_Z(size_z),
		_DELTA_X(delta_x)
	 {
		
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

	int getHashFuncParticles(float  x, float  y, float z) {
		return( _SIZE_Z+2)*(_SIZE_Y+2) *int(x / _DELTA_X) + (_SIZE_Z + 2) *int(y / _DELTA_X) +  int(z / _DELTA_X);
	};

	int getHashFunc(int x, int y, int z) {
		return (_SIZE_Z+2) * (_SIZE_Y+2) * x + (_SIZE_Z+2) * y  +   z ;
	};

	Cell* getCell(int x, int y, int z);
	Cell* getCell(int f);
	bool cellCheck(int x, int y, int z);
	bool cellCheck(int f);
	void validCells();
};

#endif