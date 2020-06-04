#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "SimDLL.h"
#include "Struct.h"
#include <random>

static unsigned int count_ = 0;
static unsigned int frames_ = 0;
static float xSpan_ = 0;
static float ySpan_ = 0;
static float zSpan_ = 0;
static unsigned int  temp_ = 1;

class Particle
{
    // Access specifier 
public:
    // Data Members 
    float* x;
    float* z;
    float* y;
    float* xVelocity;
    float* zVelocity;
    float nextX;
    float nextZ;
    float nextXVelocity;
    float nextZVelocity;

    Particle() {
        x = (float*)malloc(sizeof(float) * frames_ * temp_);
        y = (float*)malloc(sizeof(float) * frames_ * temp_);
        z = (float*)malloc(sizeof(float) * frames_ * temp_);
        xVelocity = (float*)malloc(sizeof(float) * frames_ * temp_);
        zVelocity = (float*)malloc(sizeof(float) * frames_ * temp_);

        //initializing values
        for (unsigned int i = 0; i < frames_ * temp_; i++)
        {
            x[i] = 1;
            z[i] = 1;
            y[i] = 1;
            xVelocity[i] = 0;
            zVelocity[i] = 0;
        }

      
        nextXVelocity = 0;
        nextZVelocity = 0;
        nextX = 0;
        nextZ = 0;
    }

  

    XYZ getXYZ() {
        XYZ p;
        p.x = x;
        p.y = y;
        p.z = z;
        return p;
    }

    
};


static Particle *particles_;

void particle_init() {


    for (unsigned int i = 0; i < count_; i++) {
        particles_[i] = Particle();
        particles_[i].x[0] = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (xSpan_)));
        particles_[i].z[0] = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (zSpan_)));
    }
    
    
    particles_[1].xVelocity[0] = float(0.05);
    particles_[1].x[0]=0.5;
    particles_[1].z[0] = float(0.95);
   
   
}

void parse_data(XYZ** out1) {
    XYZ* data1;
    data1 = (XYZ*)malloc(sizeof(XYZ) * count_);
    for (unsigned int i = 0; i < count_; i++) {
        data1[i] = particles_[i].getXYZ();
    }
        
    *out1 = data1;

}



void sim_run(int frame) {
    //calc new positions
    for (unsigned int i = 0; i < count_; i++) {
        particles_[i].x[frame] = particles_[i].x[frame - 1] + (particles_[i].xVelocity[frame - 1] / temp_); //time displacement
        particles_[i].z[frame] = particles_[i].z[frame - 1] + (particles_[i].zVelocity[frame - 1] / temp_);
        particles_[i].xVelocity[frame] = particles_[i].xVelocity[frame - 1];
        particles_[i].zVelocity[frame] = particles_[i].zVelocity[frame - 1];
    }
    //check for collision

    for (unsigned int i = 0; i < count_; i++) {
        for (unsigned int j = 0; j < count_; j++) {
            if (j != i) {
                double a = pow((particles_[i].x[frame] - particles_[j].x[frame]), 2);
                double b= pow((particles_[i].z[frame] - particles_[j].z[frame]), 2);
                double dist = a + b;
                if (pow(dist ,0.5)<0.1) {
                    float dxy1 = (particles_[i].xVelocity[frame-1] - particles_[j].xVelocity[frame - 1]) * (particles_[i].x[frame] - particles_[j].x[frame]) + (particles_[i].zVelocity[frame - 1] - particles_[j].zVelocity[frame - 1]) * (particles_[i].z[frame] - particles_[j].z[frame]);
                    //float dxy2 = (particles_[j].xVelocity[frame - 1] - particles_[i].xVelocity[frame - 1]) * (particles_[j].x[frame - 1] - particles_[i].x[frame - 1]) + (particles_[j].zVelocity[frame - 1] - particles_[i].zVelocity[frame - 1]) * (particles_[j].z[frame - 1] - particles_[i].z[frame - 1]);
                    particles_[i].xVelocity[frame] = particles_[i].xVelocity[frame-1] - (dxy1 * (particles_[i].x[frame - 1] - particles_[j].x[frame - 1]) / float(dist));
                    particles_[i].zVelocity[frame] = particles_[i].zVelocity[frame - 1] - (dxy1 * (particles_[i].z[frame - 1] - particles_[j].z[frame - 1]) / float(dist));
                }
            }

            

        }
        if (particles_[i].x[frame] > xSpan_ || particles_[i].x[frame] < 0) {
            particles_[i].xVelocity[frame] = -particles_[i].xVelocity[frame];
        }
        if (particles_[i].z[frame] > zSpan_ || particles_[i].z[frame] < 0) {
            particles_[i].zVelocity[frame] = -particles_[i].zVelocity[frame];
        }
    }
}

void sim_init(const int count, const int frames, const int temp, const float xSpan, const float ySpan, const float zSpan, XYZ** out1) {
    frames_ = frames;
    xSpan_ = xSpan;
    ySpan_ = ySpan;
    zSpan_ = zSpan;
    count_ = count;
    temp_ = temp;
    particles_ = (Particle*)malloc(sizeof(Particle) * count);
    particle_init();
    for (int f = 1; f < frames * temp; f++) {
        sim_run(f);
    }
    parse_data(out1);
}






bool sim_check() {
	return(frames_ != 0 && count_ != 0);
}

