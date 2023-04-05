#include <fstream>
#include <iostream>

#include "photon_sim/solver.hpp"
#include "utils/const.hpp"

using namespace std;

SolverRK* solve(double a, double init_vx, double init_vy) {
  double view_theta = Pi / 2;
  double view_r = 100.;

  BlackHole bh = BlackHole(1, a);
  VectorBL* init_r = new VectorBL(view_r, view_theta, 0., a);
  VectorCartesian* init_v = new VectorCartesian(init_vx, init_vy, 1.);
  SolverRK* solver = new SolverRK(&bh, init_r, init_v);
  solver->solve(2000, -0.1);
  return solver;
}
double intesity(vector<SolverRK*>& solvers) {
  auto make_angle = [](SolverRK* solver) {
    double phi = solver->final_r->phi;
    double theta = solver->final_r->theta;
    double phidot = solver->final_v->phi;
    double theatdot = solver->final_v->theta;
  };
  return 0.;
}

int main() {
  double d = 1e-5;
  int N = 100;
  vector<vector<double>> img(100);
  for (double init_vy = 0; init_vy < 1; init_vy += 1 / N) {
    for (double init_vx = 0; init_vx < 1; init_vx += 1 / N) {
      /**
       * Approach: assume homogeneous background
       * at each pixel, make small variation of outgoing light
       * intensity \propto angle
       * */
      SolverRK* solver1 = solve(0., init_vx - d, init_vy);
      SolverRK* solver2 = solve(0., init_vx + d, init_vy);
      SolverRK* solver3 = solve(0., init_vx, init_vy - d);
      SolverRK* solver4 = solve(0., init_vx, init_vy + d);

      img.back().push_back(
          intensity(vector<SolverRK*>{solver1, solver2, solver3, solver4}));
    }
  }

  char s[100];
  sprintf(s, "./../data/image");

  ofstream file;
  file.open(s);
  file.clear();
  for (vector<double> v : img) {
    file << bl.r << "," << bl.theta << "," << bl.phi << endl;
  }
  file.close();

  return 0;
}