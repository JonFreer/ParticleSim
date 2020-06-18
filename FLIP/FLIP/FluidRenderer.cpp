#include <fluidrender.h>
#include <GL/glut.h>

void FluidRenderer::drawCellsByType(int frame , CellType  type, float r, float g, float b , int interval) {
    int number_of_cells = cellCount[frame];
    for (int i = 0; i < number_of_cells; i++) {
        if (cells[frame * interval + i].cellType == type) {
            _drawWireframeCube(cells[frame * interval + i].x*fs->grid._DELTA_X,
                cells[frame * interval + i].y * fs->grid._DELTA_X,
                cells[frame * interval + i].z * fs->grid._DELTA_X,
                fs->grid._DELTA_X,
                r,g,b);
        }
    }
}

void FluidRenderer::drawVelocitites(int frame, int interval) {
    float h = fs->grid._DELTA_X;
    int number_of_cells = cellCount[frame];
    for (int i = 0; i < number_of_cells; i++) {
        //draw x 
        glBegin(GL_LINES);
        glColor3f(1, 1, 0.2);


        glVertex3f(cells[frame * interval + i].x+fs->grid._DELTA_X, cells[frame * interval + i].z + h / 2, cells[frame * interval + i].y + h / 2);
        glVertex3f(cells[frame * interval + i].x+fs->grid._DELTA_X + 1* cells[frame * interval + i].Velocity.x, cells[frame * interval + i].z + h / 2, cells[frame * interval + i].y + h / 2);

        glVertex3f(cells[frame * interval + i].x + h/2, cells[frame * interval + i].z + fs->grid._DELTA_X, cells[frame * interval + i].y + h / 2);
        glVertex3f(cells[frame * interval + i].x + h / 2, cells[frame * interval + i].z + fs->grid._DELTA_X  + cells[frame * interval + i].Velocity.z, cells[frame * interval + i].y + h / 2);
        
        glVertex3f(cells[frame * interval + i].x + h / 2, cells[frame * interval + i].z + h / 2, cells[frame * interval + i].y + fs->grid._DELTA_X );
        glVertex3f(cells[frame * interval + i].x + h / 2, cells[frame * interval + i].z + h / 2, cells[frame * interval + i].y + fs->grid._DELTA_X  + cells[frame * interval + i].Velocity.y);

        glEnd();
    }
}


void FluidRenderer::getCenter(float* x, float* z, float* y) {
    *x = 0.5 * fs->grid._DELTA_X * fs->grid._SIZE_X ;
    *y = 0.5 * fs->grid._DELTA_X * fs->grid._SIZE_Y;
    *z = 0.5 * fs->grid._DELTA_X * fs->grid._SIZE_Z ;
}

void FluidRenderer::drawBoundingBox(){

    float hw = 0.5 * fs->grid._DELTA_X * fs->grid._SIZE_X;
    float hd = 0.5 * fs->grid._DELTA_X * fs->grid._SIZE_Y;
    float hh = 0.5 * fs->grid._DELTA_X * fs->grid._SIZE_Z;

    float x = hw;
    float z = hd;
    float y = hh;

    glColor3f(0.2, 0.2, 0.2);

 /*   int x = hw;
    int y = hd;
    int z = hh;*/

    glBegin(GL_LINES);
    glVertex3f(x - hw, y - hh, z - hd);
    glVertex3f(x + hw, y - hh, z - hd);
    glVertex3f(x - hw, y - hh, z - hd);
    glVertex3f(x - hw, y + hh, z - hd);
    glVertex3f(x - hw, y - hh, z - hd);
    glVertex3f(x - hw, y - hh, z + hd);

    glVertex3f(x + hw, y + hh, z + hd);
    glVertex3f(x - hw, y + hh, z + hd);
    glVertex3f(x + hw, y + hh, z + hd);
    glVertex3f(x + hw, y - hh, z + hd);
    glVertex3f(x + hw, y + hh, z + hd);
    glVertex3f(x + hw, y + hh, z - hd);

    glVertex3f(x - hw, y + hh, z + hd);
    glVertex3f(x - hw, y - hh, z + hd);
    glVertex3f(x - hw, y + hh, z + hd);
    glVertex3f(x - hw, y + hh, z - hd);

    glVertex3f(x + hw, y - hh, z + hd);
    glVertex3f(x - hw, y - hh, z + hd);
    glVertex3f(x + hw, y - hh, z + hd);
    glVertex3f(x + hw, y - hh, z - hd);

    glVertex3f(x + hw, y + hh, z - hd);
    glVertex3f(x + hw, y - hh, z - hd);
    glVertex3f(x + hw, y + hh, z - hd);
    glVertex3f(x - hw, y + hh, z - hd);

    glEnd();
     
}


void FluidRenderer::drawParticles(int frame) {
    for (int i = 0; i < fs->count; i++) {
        Particle* p = &particleData[frame * fs->count + i];
        _drawSphere(p->x, p->y, p->z , 0.05);
    }
   
}

void FluidRenderer::_drawSphere(float x, float z, float y, float r) {
    glPushMatrix();
    glColor3f(1, 0.2, 0.2);
    glTranslatef(x, y, z);
    glutSolidSphere(r, 3, 3);

    glPopMatrix();
}

void FluidRenderer::_drawWireframeCube(float x_, float z_, float y_, float size ,float r, float g, float b) {
    float h = 0.5 * size;
    float x = x_ + size / 2;
    float y = y_ + size / 2;
    float z = z_ + size / 2;
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex3f(x - h, y - h, z - h);
    glVertex3f(x + h, y - h, z - h);
    glVertex3f(x - h, y - h, z - h);
    glVertex3f(x - h, y + h, z - h);
    glVertex3f(x - h, y - h, z - h);
    glVertex3f(x - h, y - h, z + h);

    glVertex3f(x + h, y + h, z + h);
    glVertex3f(x - h, y + h, z + h);
    glVertex3f(x + h, y + h, z + h);
    glVertex3f(x + h, y - h, z + h);
    glVertex3f(x + h, y + h, z + h);
    glVertex3f(x + h, y + h, z - h);

    glVertex3f(x - h, y + h, z + h);
    glVertex3f(x - h, y - h, z + h);
    glVertex3f(x - h, y + h, z + h);
    glVertex3f(x - h, y + h, z - h);

    glVertex3f(x + h, y - h, z + h);
    glVertex3f(x - h, y - h, z + h);
    glVertex3f(x + h, y - h, z + h);
    glVertex3f(x + h, y - h, z - h);

    glVertex3f(x + h, y + h, z - h);
    glVertex3f(x + h, y - h, z - h);
    glVertex3f(x + h, y + h, z - h);
    glVertex3f(x - h, y + h, z - h);

    glEnd();
}