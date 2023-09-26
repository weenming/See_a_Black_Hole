#include <fstream>
#include <iostream>
// #define PY_SSIZE_T_CLEAN

// #include "Python.h"
#include "photon_sim/solver.hpp"
#include "utils/const.hpp"

using namespace std;

int main() {
    double b = 0.01;
    for (double init_vx = -b; init_vx <= b + 1e-5; init_vx += b * 2) {
        double a = .99;
        double init_vy = .0;

        double view_theta = Pi / 2;
        double view_r = 1000.;

        BlackHole bh = BlackHole(1, a);
        VectorBL* init_r = new VectorBL(view_r, view_theta, 0., a);
        VectorCartesian* init_v = new VectorCartesian(init_vx, init_vy, 1.);
        SolverRK solver = SolverRK(&bh, init_r, init_v);

        solver.solve(20000, -0.1);

        ofstream file;

        char s[100];
        sprintf(s, "./../data/vx%.3f_vy%.3f_a%.3f_theta%.3f", init_vx, init_vy,
                a, view_theta);

        file.open(s);
        file.clear();
        for (VectorBL bl : *(solver.trajectory)) {
            file << bl.r << "," << bl.theta << "," << bl.phi << endl;
        }
        file.close();
    }
    return 0;
}