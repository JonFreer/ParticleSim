#include <iostream>

#include <Eigen/Sparse>
#include <cstdlib>
#include <ctime>
#include <solver.h>
#include <Eigen\Core>
#include <vector>

    //Solver(){}
	Solver::Solver(int count_,int size_x, int size_y, int size_z, float cell_size):grid(MacGrid(size_x, size_y, size_z,cell_size)) {
        srand(static_cast <unsigned> (time(0)));
        count = count_; //number of particles
        float len_x = 10;
        float len_y = 10;
        float len_z = 10;
        particles = (Particle*)malloc(sizeof(Particle) * count);
        for (unsigned int i = 0; i < count; i++) { //initiate Particles
            particles[i] = Particle(2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 6)),
                2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 6)),  2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 6)));
        }
        particles[0].z = 9.9;
    };

   

    void Solver::run(float delta_t, int frames , Particle* data, Cell* gridData, int* cellCount) {
        for (unsigned int f = 0; f < frames; f++) {
            std::cout << "FRAME: " << f << "\n";
            grid.update(particles, count);
            copyData(f, data, gridData, cellCount);
            //grid.validCells();
            backTrace(delta_t);
            //grid.validCells();
            applyGravity(delta_t);
            //grid.validCells();
            //applyViscoity(1, delta_t);
            //grid.validCells();
            pressureSolve(delta_t);
            //grid.validCells();
            extrapolateVelocities();
            //grid.validCells();
            checkForSolid();
            //grid.validCells();
            forwardTrace(delta_t);
           // grid.validCells();
            
        }
    }

   
   
    void Solver::copyData(int frame, Particle* data, Cell* gridData, int* cellCount) {
        for (unsigned int i = 0; i < count; i++) {
            data[i + count * frame] = particles[i];
        }
        int n_cells=0;
        for (std::pair<int, Cell> element : grid.htmap) {
            gridData[n_cells + 40000 * frame] = element.second;//200 cells per grid
            n_cells++;
        }
        cellCount[frame] = n_cells;
    }


    float Solver::getInterpolatedValueX(float x, float y, float z) {
        int i = floor(x);
        int j = floor(y);
        int k = floor(z);

        float sum = 0;
        int count = 0;

        if (grid.cellCheck(i, j, k)) {
            sum = sum + (i + 1 - x) * (j + 1 - y) * (k + 1 - z) * grid.getCell(i, j, k)->Velocity.x;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j, k)) {
            sum = sum + (x - i) * (j + 1 - y) * (k + 1 - z) * grid.getCell(i + 1, j, k)->Velocity.x;
            count = count + 1;
        }
        if (grid.cellCheck(i, j + 1, k)) {
            sum = sum + (i + 1 - x) * (y - j) * (k + 1 - z) * grid.getCell(i, j + 1, k)->Velocity.x;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j + 1, k)) {
            sum = sum + (x - i) * (y - j) * (k + 1 - z) * grid.getCell(i + 1, j + 1, k)->Velocity.x;
            count = count + 1;
        }
        if (grid.cellCheck(i, j, k + 1)) {
            sum = sum + (i + 1 - x) * (j + 1 - y) * (z - k) * grid.getCell(i, j, k + 1)->Velocity.x;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j, k + 1)) {
            sum = sum + (x - i) * (j + 1 - y) * (z - k) * grid.getCell(i + 1, j, k + 1)->Velocity.x;
            count = count + 1;
        }
        if (grid.cellCheck(i, j + 1, k + 1)) {
            sum = sum + (i + 1 - x) * (y - j) * (z - k) * grid.getCell(i, j + 1, k + 1)->Velocity.x;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j + 1, k + 1)) {
            sum = sum + (x - i) * (y - j) * (z - k) * grid.getCell(i + 1, j + 1, k + 1)->Velocity.x;
            count = count + 1;
        }

        return (sum / count) * 8;
    }

    float Solver::getInterpolatedValueY(float x, float y, float z) {
        int i = floor(x);
        int j = floor(y);
        int k = floor(z);

        float sum = 0;
        int count = 0;

        if (grid.cellCheck(i, j, k)) {
            sum = sum + (i + 1 - x) * (j + 1 - y) * (k + 1 - z) * grid.getCell(i, j, k)->Velocity.y;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j, k)) {
            sum = sum + (x - i) * (j + 1 - y) * (k + 1 - z) * grid.getCell(i + 1, j, k)->Velocity.y;
            count = count + 1;
        }
        if (grid.cellCheck(i, j + 1, k)) {
            sum = sum + (i + 1 - x) * (y - j) * (k + 1 - z) * grid.getCell(i, j + 1, k)->Velocity.y;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j + 1, k)) {
            sum = sum + (x - i) * (y - j) * (k + 1 - z) * grid.getCell(i + 1, j + 1, k)->Velocity.y;
            count = count + 1;
        }
        if (grid.cellCheck(i, j, k + 1)) {
            sum = sum + (i + 1 - x) * (j + 1 - y) * (z - k) * grid.getCell(i, j, k + 1)->Velocity.y;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j, k + 1)) {
            sum = sum + (x - i) * (j + 1 - y) * (z - k) * grid.getCell(i + 1, j, k + 1)->Velocity.y;
            count = count + 1;
        }
        if (grid.cellCheck(i, j + 1, k + 1)) {
            sum = sum + (i + 1 - x) * (y - j) * (z - k) * grid.getCell(i, j + 1, k + 1)->Velocity.y;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j + 1, k + 1)) {
            sum = sum + (x - i) * (y - j) * (z - k) * grid.getCell(i + 1, j + 1, k + 1)->Velocity.y;
            count = count + 1;
        }

        return (sum / count) * 8;
    }

    float Solver::getInterpolatedValueZ(float x, float y, float z) {
        int i = floor(x);
        int j = floor(y);
        int k = floor(z);

        float sum = 0;
        int count = 0;

        if (grid.cellCheck(i, j, k)) {
            sum = sum + (i + 1 - x) * (j + 1 - y) * (k + 1 - z) * grid.getCell(i, j, k)->Velocity.z;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j, k)) {
            sum = sum + (x - i) * (j + 1 - y) * (k + 1 - z) * grid.getCell(i + 1, j, k)->Velocity.z;
            count = count + 1;
        }
        if (grid.cellCheck(i, j + 1, k)) {
            sum = sum + (i + 1 - x) * (y - j) * (k + 1 - z) * grid.getCell(i, j + 1, k)->Velocity.z;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j + 1, k)) {
            sum = sum + (x - i) * (y - j) * (k + 1 - z) * grid.getCell(i + 1, j + 1, k)->Velocity.z;
            count = count + 1;
        }
        if (grid.cellCheck(i, j, k + 1)) {
            sum = sum + (i + 1 - x) * (j + 1 - y) * (z - k) * grid.getCell(i, j, k + 1)->Velocity.z;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j, k + 1)) {
            sum = sum + (x - i) * (j + 1 - y) * (z - k) * grid.getCell(i + 1, j, k + 1)->Velocity.z;
            count = count + 1;
        }
        if (grid.cellCheck(i, j + 1, k + 1)) {
            sum = sum + (i + 1 - x) * (y - j) * (z - k) * grid.getCell(i, j + 1, k + 1)->Velocity.z;
            count = count + 1;
        }
        if (grid.cellCheck(i + 1, j + 1, k + 1)) {
            sum = sum + (x - i) * (y - j) * (z - k) * grid.getCell(i + 1, j + 1, k + 1)->Velocity.z;
            count = count + 1;
        }

        return (sum / count) * 8;
    }

    //Trace a particle from point(float x, float y, float z) for t time using RK2

    Vector Solver::getVelocity(float x, float y, float z) {
        Vector V;
        V.x = getInterpolatedValueX(x / grid._DELTA_X, y / grid._DELTA_X - 0.5, z / grid._DELTA_X - 0.5);
        V.y = getInterpolatedValueY(x / grid._DELTA_X - 0.5, y / grid._DELTA_X, z / grid._DELTA_X - 0.5);
        V.z = getInterpolatedValueZ(x / grid._DELTA_X - 0.5, y / grid._DELTA_X - 0.5, z / grid._DELTA_X);
        return V;
    }


    Point Solver::traceParticle(float x, float y, float z, float t) {
        Vector V = getVelocity(x, y, z);
        V = getVelocity(x + (0.5 * t * V.x), y + (0.5 * t * V.y), z + (0.5 * t * V.z));
        Point p;
        p.x = x + t * V.x;
        p.y = y + t * V.y;
        p.z = z + t * V.z;
        return p;
    }


    void Solver::backTrace(float delta_t) {
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                
                Point p = traceParticle((i->second.x * grid._DELTA_X) + 0.5* grid._DELTA_X, (i->second.y * grid._DELTA_X) + 0.5 * grid._DELTA_X, (i->second.z * grid._DELTA_X) + 0.5 * grid._DELTA_X, -delta_t); //offest by 1/2 h to get to center of cell
                if (grid.htmap.find(grid.getHashFuncParticles(p.x, p.y, p.z)) != grid.htmap.end()) {
                    Cell* c = grid.getCell(int(p.x / grid._DELTA_X), int(p.y / grid._DELTA_X), int(p.z / grid._DELTA_X));
                    i->second.TempVelocity.x = c->Velocity.x;
                    i->second.TempVelocity.y = c->Velocity.y;
                    i->second.TempVelocity.z = c->Velocity.z;
                }
                else {
                    i->second.TempVelocity.x = i->second.Velocity.x;
                    i->second.TempVelocity.y = i->second.Velocity.y;
                    i->second.TempVelocity.z = i->second.Velocity.z;
                }
            }
        }

        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                i->second.Velocity.x = i->second.TempVelocity.x;
                i->second.Velocity.y = i->second.TempVelocity.y;
                i->second.Velocity.z = i->second.TempVelocity.z;
            }
        }
    }

    void Solver::forwardTrace(float delta_t) {
        for (unsigned int i = 0; i < count; i++) {
            if (particles[i].x > 0 && particles[i].y > 0 && particles[i].z >= 0) {
                Point p = traceParticle(particles[i].x, particles[i].y, particles[i].z, delta_t);
                particles[i].x = p.x;
                particles[i].y = p.y;
                particles[i].z = p.z;
            }
        }
    }

    void Solver::applyViscoity(float viscosity, float delta_t) {
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) { //all fluid cells should have neighbours
                float tempX = 0;
                float tempY = 0;
                float tempZ = 0;
                Cell* c = grid.getCell(i->second.x + 1, i->second.y, i->second.z);
                if (c->cellType == FLUID) {
                    tempX = c->Velocity.x - i->second.Velocity.x;
                    tempY = c->Velocity.y - i->second.Velocity.y;
                    tempZ = c->Velocity.z - i->second.Velocity.z;
                }
                c = grid.getCell(i->second.x - 1, i->second.y, i->second.z);
                if (c->cellType == FLUID) {
                    tempX = c->Velocity.x - i->second.Velocity.x;
                    tempY = c->Velocity.y - i->second.Velocity.y;
                    tempZ = c->Velocity.z - i->second.Velocity.z;
                }
                c = grid.getCell(i->second.x, i->second.y + 1, i->second.z);
                if (c->cellType == FLUID) {
                    tempX = c->Velocity.x - i->second.Velocity.x;
                    tempY = c->Velocity.y - i->second.Velocity.y;
                    tempZ = c->Velocity.z - i->second.Velocity.z;
                }
                c = grid.getCell(i->second.x, i->second.y - 1, i->second.z);
                if (c->cellType == FLUID) {
                    tempX = c->Velocity.x - i->second.Velocity.x;
                    tempY = c->Velocity.y - i->second.Velocity.y;
                    tempZ = c->Velocity.z - i->second.Velocity.z;
                }
                c = grid.getCell(i->second.x, i->second.y, i->second.z + 1);
                if (c->cellType == FLUID) {
                    tempX = c->Velocity.x - i->second.Velocity.x;
                    tempY = c->Velocity.y - i->second.Velocity.y;
                    tempZ = c->Velocity.z - i->second.Velocity.z;
                }
                c = grid.getCell(i->second.x, i->second.y, i->second.z - 1);
                if (c->cellType == FLUID) {
                    tempX = c->Velocity.x - i->second.Velocity.x;
                    tempY = c->Velocity.y - i->second.Velocity.y;
                    tempZ = c->Velocity.z - i->second.Velocity.z;
                }

                i->second.TempVelocity.x = i->second.Velocity.x + (tempX * delta_t * viscosity);
                i->second.TempVelocity.y = i->second.Velocity.y + (tempY * delta_t * viscosity);
                i->second.TempVelocity.z = i->second.Velocity.z + (tempZ * delta_t * viscosity);


            }
        }
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                i->second.Velocity.x = i->second.TempVelocity.x;
                i->second.Velocity.y = i->second.TempVelocity.y;
                i->second.Velocity.z = i->second.TempVelocity.z;
            }
        }
    }

    void Solver::applyGravity(float delta_t) {

        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                i->second.Velocity.z -= delta_t * 1;
            }
            else {
                if (grid.cellCheck(i->second.x, i->second.y, i->second.z + 1)) {
                    if (grid.getCell(i->second.x, i->second.y, i->second.z + 1)->cellType == FLUID) {
                        i->second.Velocity.z -= delta_t * 1;
                    }
                }
            }
        }


    }



    void Solver::pressureSolve(float delta_t) {
        Eigen::SparseMatrix<float> _A;

        Eigen::VectorXf pressure;
        Eigen::ConjugateGradient<Eigen::SparseMatrix<float> > _cg_solver;

        int n_fluid_cells = 0;
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                i->second.index = n_fluid_cells; //set index of cells
                n_fluid_cells++; //count the number of fluid cells to detrermin matrix size
            }
        }
        if (n_fluid_cells == 0) {
            return;
        } 


        for (unsigned int i = 0; i < count; i++) { //work out densities     
            if (particles[i].x >= 0 && particles[i].y >= 0 && particles[i].z >= 0) {
                if (grid.cellCheck(grid.getHashFuncParticles(particles[i].x, particles[i].y, particles[i].z))) { //if cell exists
                    grid.getCell(grid.getHashFuncParticles(particles[i].x, particles[i].y, particles[i].z))->density++;
                }
            }
        }


        _A.resize(n_fluid_cells, n_fluid_cells);
        _A.reserve(Eigen::VectorXi::Constant(n_fluid_cells,7)); //reserve space for 7 non 0 entries per row
        Eigen::VectorXf b(n_fluid_cells);


        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                int n_non_solid_neighbors = 0;
                int n_air_cells = 0;
                float divergence = 0;
                Cell* c = grid.getCell(i->second.x + 1, i->second.y, i->second.z);
                if (c->cellType != SOLID) {
                    if (c->cellType == FLUID) {
                        _A.insert(i->second.index,c->index) = 1;
                        
                    }
                    else {
                        n_air_cells++;
                    }
                    divergence = divergence + c->Velocity.x - i->second.Velocity.x;
                    n_non_solid_neighbors++;
                }
                c = grid.getCell(i->second.x - 1, i->second.y, i->second.z);
                if (c->cellType != SOLID) {
                    if (c->cellType == FLUID) {
                        _A.insert(i->second.index, c->index) = 1;
                    }
                    else {
                        n_air_cells++;
                    }
                    n_non_solid_neighbors++;
                }
                c = grid.getCell(i->second.x, i->second.y + 1, i->second.z);
                if (c->cellType != SOLID) {
                    if (c->cellType == FLUID) {
                        _A.insert(i->second.index, c->index) = 1;
                    }
                    else {
                        n_air_cells++;
                    }
                    n_non_solid_neighbors++;
                    divergence = divergence + c->Velocity.y - i->second.Velocity.y;
                    
                }
                c = grid.getCell(i->second.x, i->second.y - 1, i->second.z);
                if (c->cellType != SOLID) {
                    if (c->cellType == FLUID) {
                        _A.insert(i->second.index, c->index) = 1;
                    }
                    else {
                        n_air_cells++;
                    }
                    n_non_solid_neighbors++;
                }
                c = grid.getCell(i->second.x, i->second.y, i->second.z + 1);
                if (c->cellType != SOLID) {
                    if (c->cellType == FLUID) {
                        _A.insert(i->second.index, c->index) = 1;
                        
                    }
                    else {
                        n_air_cells++;
                    }
                    divergence = divergence + c->Velocity.z - i->second.Velocity.z;
                    n_non_solid_neighbors++;
                }
                c = grid.getCell(i->second.x, i->second.y, i->second.z - 1);
                if (c->cellType != SOLID) {
                    if (c->cellType == FLUID) {
                        _A.insert(i->second.index, c->index) = 1;
                    }
                    else {
                        n_air_cells++;
                    }
                    n_non_solid_neighbors++;
                }
                b[i->second.index] = ((grid._DELTA_X * i->second.density )/ delta_t) * divergence - n_air_cells;
                _A.insert(i->second.index, i->second.index) = -n_non_solid_neighbors;

            }
        }

        //solve pressure
        _cg_solver.compute(_A);
        pressure = _cg_solver.solve(b);



        //apply pressure
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                float divergenceX = 0;
                float divergenceY = 0;
                float divergenceZ = 0;
                Cell* c = grid.getCell(i->second.x - 1, i->second.y, i->second.z);
                if (c->cellType == FLUID) {
                    divergenceX = pressure[i->second.index] - pressure[c->index];
                }
                else if (i->second.cellType == AIR) {
                   // divergenceX = pressure[i->second.index] - 1; 
                }
                c = grid.getCell(i->second.x, i->second.y - 1, i->second.z);
                if (c->cellType == FLUID) {
                    divergenceY = pressure[i->second.index] - pressure[c->index];
                }
                else if (i->second.cellType == AIR) {
                   // divergenceY = pressure[i->second.index] - 1;
                }
                c = grid.getCell(i->second.x, i->second.y, i->second.z - 1);
                if (c->cellType == FLUID) {
                    divergenceZ = pressure[i->second.index] - pressure[c->index];
                }
                else if (i->second.cellType == AIR) {
                 //   divergenceZ = pressure[i->second.index] - 1;
                }
                i->second.Velocity.x = i->second.Velocity.x - divergenceX * (delta_t / (grid._DELTA_X *i->second.density));
                i->second.Velocity.y = i->second.Velocity.y - divergenceY * (delta_t / (grid._DELTA_X * i->second.density));
                i->second.Velocity.z = i->second.Velocity.z - divergenceZ * (delta_t / (grid._DELTA_X * i->second.density));
            }
            else if (i->second.cellType == AIR) { //air cells are considered to be at atmospheric pressure ie 1
                float divergenceX = 0;
                float divergenceY = 0;
                float divergenceZ = 0;
                if (grid.cellCheck(i->second.x + 1, i->second.y, i->second.z)) {
                    Cell* c = grid.getCell(i->second.x + 1, i->second.y, i->second.z);
                    if (c->cellType == FLUID) {
                        divergenceX = 1- pressure[c->index] ;
                    }
                }
                if (grid.cellCheck(i->second.x , i->second.y+1, i->second.z)) {
                    Cell* c = grid.getCell(i->second.x, i->second.y +1 , i->second.z);
                    if (c->cellType == FLUID) {
                        divergenceY = 1- pressure[c->index] ;
                    }
                }
                if (grid.cellCheck(i->second.x, i->second.y, i->second.z+1)) {
                    Cell* c = grid.getCell(i->second.x, i->second.y, i->second.z+1);
                    if (c->cellType == FLUID) {
                        divergenceZ = 1- pressure[c->index] ;
                    }
                }
             
               // i->second.Velocity.x = i->second.Velocity.x - divergenceX * (delta_t / grid._DELTA_X);
                //i->second.Velocity.y = i->second.Velocity.y - divergenceY * (delta_t / grid._DELTA_X);
               // i->second.Velocity.z = i->second.Velocity.z - divergenceZ * (delta_t / grid._DELTA_X);
            }

        }
    }

    void Solver::extrapolateVelocitiesHelper(int x, int y, int z, float* sumVelocityX, float* sumVelocityY, float* sumVelocityZ, int* neighbour_count, int i) {
        if (grid.htmap.find(grid.getHashFunc(x, y, z)) != grid.htmap.end()) { // if n exisits
            Cell* c = grid.getCell(x, y, z);
            if (c->layer == (i - 1)) {
                (*neighbour_count)++;
                *sumVelocityX = *sumVelocityX + c->Velocity.x;
                *sumVelocityY = *sumVelocityY + c->Velocity.y;
                *sumVelocityZ = *sumVelocityZ + c->Velocity.z;
            }
        }
    }


    void Solver::_resetExtraoplatedFluidVelocities() {
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType != FLUID) {
                if (grid.cellCheck(i->second.x + 1, i->second.y, i->second.z)) {
                    if (grid.getCell(i->second.x + 1, i->second.y, i->second.z)->cellType != FLUID) {
                        i->second.Velocity.x = 0;
                    }
                }
                if (grid.cellCheck(i->second.x , i->second.y + 1, i->second.z)) {
                    if (grid.getCell(i->second.x, i->second.y + 1, i->second.z)->cellType != FLUID) {
                        i->second.Velocity.y = 0;
                    }
                }
                if (grid.cellCheck(i->second.x , i->second.y, i->second.z+1)) {
                    if (grid.getCell(i->second.x , i->second.y, i->second.z+1)->cellType != FLUID) {
                        i->second.Velocity.z = 0;
                    }
                }
            }
        }
    }

    void Solver::extrapolateVelocities() {
        //_resetExtraoplatedFluidVelocities();
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                i->second.layer = 0;
            }
            else {
                i->second.layer = -1;
            }
        }
        for (int x = 1; x < 3; x++) { //2 is user defined constant
            for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
                if (i->second.layer == -1) {
                    float sumVelocityX = 0;
                    float sumVelocityY = 0;
                    float sumVelocityZ = 0;
                    int neighbour_count = 0;

                    extrapolateVelocitiesHelper(i->second.x + 1, i->second.y, i->second.z, &sumVelocityX, &sumVelocityY, &sumVelocityZ, &neighbour_count, x);
                    extrapolateVelocitiesHelper(i->second.x - 1, i->second.y, i->second.z, &sumVelocityX, &sumVelocityY, &sumVelocityZ, &neighbour_count, x);
                    extrapolateVelocitiesHelper(i->second.x, i->second.y + 1, i->second.z, &sumVelocityX, &sumVelocityY, &sumVelocityZ, &neighbour_count, x);
                    extrapolateVelocitiesHelper(i->second.x, i->second.y - 1, i->second.z, &sumVelocityX, &sumVelocityY, &sumVelocityZ, &neighbour_count, x);
                    extrapolateVelocitiesHelper(i->second.x, i->second.y, i->second.z + 1, &sumVelocityX, &sumVelocityY, &sumVelocityZ, &neighbour_count, x);
                    extrapolateVelocitiesHelper(i->second.x, i->second.y, i->second.z - 1, &sumVelocityX, &sumVelocityY, &sumVelocityZ, &neighbour_count, x);
                    if (neighbour_count != 0) {
                        if (grid.cellCheck(i->second.x+1, i->second.y, i->second.z)) {
                            if (grid.getCell(i->second.x+1, i->second.y, i->second.z)->cellType!=FLUID) {
                                i->second.Velocity.x = sumVelocityX / neighbour_count;
                            }
                        }
                        if (grid.cellCheck(i->second.x, i->second.y+1, i->second.z)) {
                            if (grid.getCell(i->second.x, i->second.y+1, i->second.z)->cellType != FLUID) {
                                i->second.Velocity.y = sumVelocityY / neighbour_count;
                            }
                        }
                        if (grid.cellCheck(i->second.x, i->second.y, i->second.z+1)) {
                            if (grid.getCell(i->second.x, i->second.y, i->second.z+1)->cellType != FLUID) {
                                i->second.Velocity.z = sumVelocityZ / neighbour_count;
                            }
                        }
                      
                        i->second.Velocity.x = sumVelocityX / neighbour_count;
                        i->second.Velocity.y = sumVelocityY / neighbour_count;
                        i->second.Velocity.z = sumVelocityZ / neighbour_count;
                        i->second.layer = x;
                    }
                }
            }
        }
    }

    void Solver::checkForSolid() {
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType != SOLID) {

                if (i->second.Velocity.x > 0) {
                    if (grid.htmap.find(grid.getHashFunc(i->second.x + 1, i->second.y, i->second.z)) != grid.htmap.end()) { // if n exisits
                        Cell* c = grid.getCell(i->second.x + 1, i->second.y, i->second.z);
                        if (c->cellType == SOLID) {
                            i->second.Velocity.x = 0;
                        }
                    }
                }
                else if (i->second.Velocity.x < 0) {
                    if (grid.htmap.find(grid.getHashFunc(i->second.x - 1, i->second.y, i->second.z)) != grid.htmap.end()) { // if n exisits
                        Cell* c = grid.getCell(i->second.x - 1, i->second.y, i->second.z);
                        if (c->cellType == SOLID) {
                            i->second.Velocity.x = 0;
                        }
                    }
                }

                if (i->second.Velocity.y > 0) {
                    if (grid.htmap.find(grid.getHashFunc(i->second.x, i->second.y + 1, i->second.z)) != grid.htmap.end()) { // if n exisits
                        Cell* c = grid.getCell(i->second.x, i->second.y + 1, i->second.z);
                        if (c->cellType == SOLID) {
                            i->second.Velocity.y = 0;
                        }
                    }
                }
                else if (i->second.Velocity.y < 0) {
                    if (grid.htmap.find(grid.getHashFunc(i->second.x, i->second.y - 1, i->second.z)) != grid.htmap.end()) { // if n exisits
                        Cell* c = grid.getCell(i->second.x, i->second.y - 1, i->second.z);
                        if (c->cellType == SOLID) {
                            i->second.Velocity.y = 0;
                        }
                    }
                }

                if (i->second.Velocity.z > 0) {
                    if (grid.htmap.find(grid.getHashFunc(i->second.x, i->second.y, i->second.z + 1)) != grid.htmap.end()) { // if n exisits
                        Cell* c = grid.getCell(i->second.x, i->second.y, i->second.z + 1);
                        if (c->cellType == SOLID) {
                            i->second.Velocity.z = 0;
                        }
                    }
                }
                else if (i->second.Velocity.z < 0) {
                    if (grid.htmap.find(grid.getHashFunc(i->second.x, i->second.y, i->second.z - 1)) != grid.htmap.end()) { // if n exisits
                        Cell* c = grid.getCell(i->second.x, i->second.y, i->second.z - 1);
                        if (c->cellType == SOLID) {
                            i->second.Velocity.z = 0;
                        }
                    }
                }

            }
        }
    }

    void Solver::outputParticles() {
        for (unsigned int i = 0; i < count; i++) {
            std::cout << i << ": " << particles[i].x << ", " << particles[i].y << ", " << particles[i].z << "\n";
        }
    }

