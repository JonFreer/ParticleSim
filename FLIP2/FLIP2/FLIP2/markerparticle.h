#pragma once
class Particle {
public:
	Particle(float x, float y) : X(x), Y(y) {

	};

	float X;
	float Y;

	void getCell(int* x, int* y, float DX) {
		*x = int(X / DX);
		*y = int(Y / DX);
	};
};