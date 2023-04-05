#include <fstream>
#include <iostream>
// #define PY_SSIZE_T_CLEAN

// #include "Python.h"
#include "photon_sim/solver.hpp"
#include "utils/const.hpp"

using namespace std;

int main() {
  for (double init_vy = 0; init_vy < 1; init_vy += 0.1) {
    double a = 1;
    double init_vx = 0.0;

    double view_theta = Pi / 2;
    double view_r = 100.;

    BlackHole bh = BlackHole(1, a);
    VectorBL* init_r = new VectorBL(view_r, view_theta, 0., a);
    VectorCartesian* init_v = new VectorCartesian(init_vx, init_vy, 1.);
    SolverRK solver = SolverRK(&bh, init_r, init_v);

    solver.solve(2000, -0.1);

    ofstream file;

    char s[100];
    sprintf(s, "./../data/trajectory_vx%.10f_vy%.10f_a%.10f_theta%.10f",
            init_vx, init_vy, a, view_theta);

    file.open(s);
    file.clear();
    for (VectorBL bl : *(solver.trajectory)) {
      file << bl.r << "," << bl.theta << "," << bl.phi << endl;
    }
    file.close();
  }
  return 0;
}