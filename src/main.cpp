
#include <iostream>
#define PY_SSIZE_T_CLEAN

#include "Python.h"
#include "photon_sim/solver.hpp"
#include "utils/const.hpp"

using namespace std;

int main() {
  double a = 0;
  BlackHole bh = BlackHole(1, a);
  VectorBL* init_r = new VectorBL(10., 0., 0., a);
  VectorCartesian* init_v = new VectorCartesian(1, 0., 1.);
  SolverRK solver = SolverRK(&bh, init_r, init_v);

  solver.solve(100, -1);
  return 0;
}