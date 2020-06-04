// Sim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<cmath>

using namespace std;

class Particle
{
    // Access specifier 
public:
    // Data Members 
    float x;
    float z;
    float xVelocity;
    float zVelocity;
    float nextX;
    float nextZ;
    float nextXVelocity;
    float nextZVelocity;

    Particle() {
        x = 0;
        z = 0;
        xVelocity = 0;
        zVelocity = 0;
        nextXVelocity = 0;
        nextZVelocity = 0;
        nextX = 0;
        nextZ = 0;
    }

    Particle(float X, float Z, float XVelocity, float ZVelocity)
    {
        x = X;
        z = Z;
        xVelocity = XVelocity;
        zVelocity = ZVelocity;
    }
};




int main()
{
    std::cout << "Hello World!\n";
    const int xMin = 0;
    const int xMax = 100;
    const int zMin = 0;
    const int zMax = 100;
    const int particleCount = 2;
    Particle particles[particleCount];
    for (int i = 0; i < particleCount; i++) {
        particles[i].x = 20;
        particles[i].z = 50;
    }
    particles[0].x = 20;
    particles[0].xVelocity = 4;
    particles[0].nextXVelocity = 4;
    particles[1].x = 17;
    particles[1].xVelocity = 1;
    particles[1].nextXVelocity = 1;

    //work out predicted positions

 

    for (int l = 0; l < 120; l++) {

        for (int i = 0; i < particleCount; i++) {
            particles[i].nextX = particles[i].x + particles[i].xVelocity;
            particles[i].nextZ = particles[i].z + particles[i].zVelocity;
        }

        for (int i = 0; i < particleCount; i++) {
            for (int j = 0; j < particleCount; j++) {
                if (i != j) {
                    if (particles[i].nextX == particles[j].nextX && particles[i].nextZ == particles[j].nextZ) { //if collision
                        particles[i].nextXVelocity = particles[j].xVelocity;
                        particles[j].nextXVelocity = particles[i].xVelocity;
                        
                    }
                }
            }
        }

        for (int i = 0; i < particleCount; i++) {
            particles[i].x = particles[i].nextX;
            particles[i].z = particles[i].nextZ;
            particles[i].xVelocity = particles[i].nextXVelocity;
            cout << "\nx:"<<i<< "val:" << particles[i].x;
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


