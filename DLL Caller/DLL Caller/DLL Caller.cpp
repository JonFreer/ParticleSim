// DLL Caller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#pragma comment(lib, "SimDLL.lib")
#include "SimDLL.h"


int main()
{

    std::cout << sim_check();
    XYZ* data;
    sim_init(100, 50,5, 2, 2, 2, &data);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 50*5; j++) {
            std::cout << "\n X:" << data[i].x[j];
            std::cout << "\n Y:" << data[i].y[j];
            std::cout << "\n Z:" << data[i].z[j];
        }
    
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
