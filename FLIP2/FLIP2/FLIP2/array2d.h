#pragma once

#include <string> 
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <string> 
template <class T>
class Array2d {
public:
	Array2d() {
		_initializeGrid();
	}

	Array2d(int i, int j):width(i),height(j) {
		_initializeGrid();
	}

	Array2d(int i, int j , T fillVal) :width(i), height(j) {
		_initializeGrid();
		fill(fillVal);
	}


	void fill(T value) {
		for (int i = 0; i < width * height; i++) {
			_grid[i] = value;
		}
	}

	void set(int x, int y, T val) {
		_grid[x + y * width] = val;
	}

	T get(int x, int y) {
		return _grid[x + y * width];
	}

	T get(int x) {
		return _grid[x];
	}

	int width = 0;
	int height = 0;

private:

	void _initializeGrid() {
		if (width <= 0 || height <= 0) {
			//std::string msg = "Error: dimensions cannot be negative or zero.\n";
			//msg += "width: " + std::_toString(width) + " height: " + std::_toString(height) + "\n";
			//throw std::domain_error(msg);
		}

		_grid = new T[width * height];
	}

	T* _grid;
};