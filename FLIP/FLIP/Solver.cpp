class Solver {

    public:

	Solver() {};

    #include <iostream>
    #include <MacGrid.h>
    #include <Particle.h>
    #include <Eigen/Sparse>

    MacGrid grid;
    Particle* particles;
    unsigned int count;



    float getInterpolatedValueX(float x, float y, float z) {
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

    float getInterpolatedValueY(float x, float y, float z) {
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

    float getInterpolatedValueZ(float x, float y, float z) {
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

    Vector getVelocity(float x, float y, float z) {
        Vector V;
        V.x = getInterpolatedValueX(x / grid._DELTA_X, y / grid._DELTA_Y - 0.5, z / grid._DELTA_Z - 0.5);
        V.y = getInterpolatedValueY(x / grid._DELTA_X - 0.5, y / grid._DELTA_Y, z / grid._DELTA_Z - 0.5);
        V.z = getInterpolatedValueZ(x / grid._DELTA_X - 0.5, y / grid._DELTA_Y - 0.5, z / grid._DELTA_Z);
        return V;
    }


    Point traceParticle(float x, float y, float z, float t) {
        Vector V = getVelocity(x, y, z);
        V = getVelocity(x + 0.5 * t * V.x, y + 0.5 * t * V.y, z + 0.5 * t * V.z);
        Point p;
        p.x = x + t * V.x;
        p.y = y + t * V.y;
        p.z = z + t * V.z;
        return p;
    }


    void backTrace(float delta_t) {
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                Point p = traceParticle(i->second.x * grid._DELTA_X, i->second.y * grid._DELTA_Y, i->second.z * grid._DELTA_Z, -delta_t);
                Cell* c = grid.getCell(int(p.x / grid._DELTA_X), int(p.y / grid._DELTA_Y), int(p.z / grid._DELTA_Z));
                i->second.TempVelocity.x = c->Velocity.x;
                i->second.TempVelocity.y = c->Velocity.y;
                i->second.TempVelocity.z = c->Velocity.z;
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

    void forwardTrace(float delta_t) {
        for (unsigned int i = 0; i < count; i++) {
            Point p = traceParticle(particles[i].x, particles[i].y, particles[i].z, delta_t);
            particles[i].x = p.x;
            particles[i].y = p.y;
            particles[i].z = p.z;
        }
    }

    void applyViscoity(float viscosity, float delta_t) {
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
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

    void applyGravity(float delta_t) {

        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                i->second.Velocity.z += delta_t * 1;
            }
        }


    }

    void pressureSolve(float delta_t) {
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
        _A.resize(n_fluid_cells, n_fluid_cells);
        _A.reserve(Eigen::VectorXi::Constant(7, n_fluid_cells)); //reserve space for 7 non 0 entries per row
        Eigen::VectorXf b(n_fluid_cells);
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                int n_non_solid_neighbors = 0;
                int n_air_cells = 0;
                float divergence = 0;
                Cell* c = grid.getCell(i->second.x + 1, i->second.y, i->second.z);
                if (c->cellType != SOLID) {
                    if (c->cellType == FLUID) {
                        _A.insert(i->second.index, c->index) = 1;
                        divergence = divergence + c->Velocity.x - i->second.Velocity.x;
                    }
                    else {
                        n_air_cells++;
                    }
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
                        divergence = divergence + c->Velocity.y - i->second.Velocity.y;
                    }
                    else {
                        n_air_cells++;
                    }
                    n_non_solid_neighbors++;
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
                        divergence = divergence + c->Velocity.z - i->second.Velocity.z;
                    }
                    else {
                        n_air_cells++;
                    }
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
                b[i->second.index] = (grid._DELTA_X / delta_t) * divergence - n_air_cells;
                _A.insert(i->second.index, i->second.index) = n_non_solid_neighbors;

            }
        }

        //solve pressure
        _cg_solver.compute(_A);
        pressure = _cg_solver.solve(b);

        //apply pressure
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                float divergence = 0;
                Cell* c = grid.getCell(i->second.x - 1, i->second.y, i->second.z);
                if (c->cellType == FLUID) {
                    divergence = pressure[i->second.index] - pressure[c->index];
                }
                c = grid.getCell(i->second.x, i->second.y - 1, i->second.z);
                if (c->cellType == FLUID) {
                    divergence = pressure[i->second.index] - pressure[c->index];
                }
                c = grid.getCell(i->second.x, i->second.y, i->second.z - 1);
                if (c->cellType == FLUID) {
                    divergence = pressure[i->second.index] - pressure[c->index];
                }
                i->second.Velocity.x = i->second.Velocity.x - divergence * (delta_t / grid._DELTA_X);
                i->second.Velocity.y = i->second.Velocity.y - divergence * (delta_t / grid._DELTA_X);
                i->second.Velocity.z = i->second.Velocity.z - divergence * (delta_t / grid._DELTA_X);
            }

        }
    }

    void extrapolateVelocitiesHelper(int x, int y, int z, float* sumVelocityX, float* sumVelocityY, float* sumVelocityZ, int* neighbour_count, int i) {
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


    void extrapolateVelocities() {
        for (auto i = grid.htmap.begin(); i != grid.htmap.end(); i++) {
            if (i->second.cellType == FLUID) {
                i->second.layer = 0;
            }
            else {
                i->second.layer = -1;
            }
        }
        for (int x = 1; x < 2; x++) { //2 is user defined constant
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
                        i->second.Velocity.x = sumVelocityX / neighbour_count;
                        i->second.Velocity.y = sumVelocityY / neighbour_count;
                        i->second.Velocity.z = sumVelocityZ / neighbour_count;
                        i->second.layer = x;
                    }
                }
            }
        }
    }

    void checkForSolid() {
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

    void outputParticles() {
        for (unsigned int i = 0; i < count; i++) {
            std::cout << i << ": " << particles[i].x << ", " << particles[i].y << ", " << particles[i].z << "\n";
        }

    }

};