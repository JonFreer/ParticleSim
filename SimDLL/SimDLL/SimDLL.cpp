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
static const unsigned int k_ = 2;
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

struct node {
    Particle* location;
    node* left;
    node* right;
};

static Particle *particles_;
static node* tree_;


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





void propogate_tree(node* tree ,Particle* p, unsigned int depth, unsigned int frame) {
    if (depth % 2 == 0) {
        if (p->x[frame] - tree->location->x[frame] < -0.1) { // node is fully to the left of particle
            if (tree->left != 0) { //explore to the left of node
                propogate_tree(tree->left, p, depth + 1, frame);
            }
            else {
                return;
            }
        }
        else if (p->x[frame] - tree->location->x[frame] > 0.1) { // node is fully to the left of particle
            if (tree->right != 0) { //explore to the right of node
                propogate_tree(tree->right, p, depth + 1, frame);
            }
            else {
                return;
            }
        }
        else {

            if (tree->left != 0) { //explore to the left of node
                propogate_tree(tree->left, p, depth + 1, frame);
            }
        
            if (tree->right != 0) { //explore to the right of node
                propogate_tree(tree->right, p, depth + 1, frame);
            }
        

            
            if (tree->location != p) { //check that its not itself
                if (p->z[frame] - tree->location->z[frame] > -0.1 && p->z[frame] - tree->location->z[frame] < 0.1) { //check if node itself is collision

                    double a = pow((p->x[frame] - tree->location->x[frame]), 2);
                    double b = pow((p->z[frame] - tree->location->z[frame]), 2);
                    double dist = a + b;
                    //if (dist<0.1) {
                    if (dist < 0.01) {
                        float dxy1 = (p->xVelocity[frame - 1] - tree->location->xVelocity[frame - 1]) * (p->x[frame] - tree->location->x[frame]) 
                            + (p->zVelocity[frame - 1] - tree->location->zVelocity[frame - 1]) * (p->z[frame] - tree->location->z[frame]);
                        
                        p->xVelocity[frame] = p->xVelocity[frame - 1] - (dxy1 * (p->x[frame - 1] - tree->location->x[frame - 1]) / float(dist));
                        p->zVelocity[frame] = p->zVelocity[frame - 1] - (dxy1 * (p->z[frame - 1] - tree->location->z[frame - 1]) / float(dist));
                    }
                }
            }

        }
    }
    else {
        if (p->z[frame] - tree->location->z[frame] < -0.1) { // node is fully to the left of particle
            if (tree->left != 0) { //explore to the left of node
                propogate_tree(tree->left, p, depth + 1, frame);
            }
            else {
                return;
            }
        }
        else if (p->z[frame] - tree->location->z[frame] > 0.1) { // node is fully to the left of particle
            if (tree->right != 0) { //explore to the right of node
                propogate_tree(tree->right, p, depth + 1, frame);
            }
            else {
                return;
            }
        }
        else {

            if (tree->left != 0) { //explore to the left of node
                propogate_tree(tree->left, p, depth + 1, frame);
            }
      
            if (tree->right != 0) { //explore to the right of node
                propogate_tree(tree->right, p, depth + 1, frame);
            }
          

            if (tree->location != p) { //check that its not itself
                if (p->x[frame] - tree->location->x[frame] > -0.1 && p->x[frame] - tree->location->x[frame] < 0.1) { //check if node itself is collision

                    double a = pow((p->x[frame] - tree->location->x[frame]), 2);
                    double b = pow((p->z[frame] - tree->location->z[frame]), 2);
                    double dist = a + b;
                    //if (dist<0.1) {
                    if (dist < 0.01) {
                        float dxy1 = (p->xVelocity[frame - 1] - tree->location->xVelocity[frame - 1]) * (p->x[frame] - tree->location->x[frame]) + (p->zVelocity[frame - 1] - tree->location->zVelocity[frame - 1]) * (p->z[frame] - tree->location->z[frame]);
                        //float dxy2 = (particles_[j].xVelocity[frame - 1] - particles_[i].xVelocity[frame - 1]) * (particles_[j].x[frame - 1] - particles_[i].x[frame - 1]) + (particles_[j].zVelocity[frame - 1] - particles_[i].zVelocity[frame - 1]) * (particles_[j].z[frame - 1] - particles_[i].z[frame - 1]);
                        p->xVelocity[frame] = p->xVelocity[frame - 1] - (dxy1 * (p->x[frame - 1] - tree->location->x[frame - 1]) / float(dist));
                        p->zVelocity[frame] = p->zVelocity[frame - 1] - (dxy1 * (p->z[frame - 1] - tree->location->z[frame - 1]) / float(dist));
                    }
                }
            }

        }
    }
}




void place_element(unsigned int n, node* tree ,unsigned int depth, unsigned int frame) {
    if (depth % 2 == 0) {
        if (particles_[n].x[frame] < tree->location->x[frame]) {
            if (tree->left == 0) {
                tree->left = &tree_[n];
            }
            else {
                place_element(n, tree->left, depth+1, frame);
            }
        }
        else {
            if (tree->right == 0) {
                tree->right = &tree_[n];
            }
            else {
                place_element(n, tree->right, depth + 1, frame);
            }
        }

    }
    else {
        if (particles_[n].z[frame] < tree->location->z[frame]) {
            if (tree->left == 0) {
                tree->left = &tree_[n];
            }
            else {
                place_element(n, tree->left, depth + 1, frame);
            }
        }
        else {
            if (tree->right == 0) {
                tree->right = &tree_[n];
            }
            else {
                place_element(n, tree->right, depth + 1, frame);
            }
        }
    }
}

void gen_tree(unsigned int frame) {
    tree_[0] = node();
    tree_[0].location = &particles_[0];
    tree_[0].left = 0;
    tree_[0].right = 0;
    for (unsigned int i = 1; i < count_; i++ ) { //for each element create a new node and try to place it
        tree_[i] = node();
        tree_[i].location = &particles_[i];
        tree_[i].left = 0;
        tree_[i].right = 0;
        place_element(i, &tree_[0],0,frame);
    }
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

    gen_tree(frame); //generate the tree

    for (unsigned int i = 0; i < count_; i++) {

        propogate_tree(&tree_[0], &particles_[i], 0, frame);

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
    tree_ = (node*)malloc(sizeof(node) * count);
    particle_init();

    

    for (int f = 1; f < frames * temp; f++) {
        sim_run(f);
    }
    parse_data(out1);
}

void sim_free() {
    free(tree_);
    for (unsigned int i = 0; i < count_; i++) {
        free(particles_[i].x);
        free(particles_[i].y);
        free(particles_[i].z);
        free(particles_[i].xVelocity);
        free(particles_[i].zVelocity);
    }
    free(particles_);
}




bool sim_check() {
	return(frames_ != 0 && count_ != 0);
}

