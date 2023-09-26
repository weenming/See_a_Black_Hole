#include <algorithm>
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

double intensity(vector<SolverRK*> solvers, double d) {
    double dtheta =
        max({solvers[0]->final_v->theta, solvers[1]->final_v->theta,
             solvers[2]->final_v->theta, solvers[3]->final_v->theta}) -
        min({solvers[0]->final_v->theta, solvers[1]->final_v->theta,
             solvers[2]->final_v->theta, solvers[3]->final_v->theta});

    double dphi = max({solvers[0]->final_v->phi, solvers[1]->final_v->phi,
                       solvers[2]->final_v->phi, solvers[3]->final_v->phi}) -
                  min({solvers[0]->final_v->phi, solvers[1]->final_v->phi,
                       solvers[2]->final_v->phi, solvers[3]->final_v->phi});
    double avgtheta = solvers[0]->final_v->theta;

    return dtheta * dphi * sin(avgtheta) / (2 * pow(d, 2));
}

bool no_null(vector<SolverRK*> solvers) {
    auto solver_no_null = [](SolverRK* s) {
        return s->final_r != NULL && s->final_v != NULL;
    };
    for (auto s : solvers) {
        if (!solver_no_null(s)) return false;
    }
    return true;
}

int main() {
    double d = 1e-5;
    int N = 100;
    double vx_max = 0.1;
    double vy_max = 0.1;
    vector<vector<double>> img;

    for (double init_vy = -vy_max; init_vy < vy_max; init_vy += vy_max / N) {
        cout << init_vy << endl;
        vector<double> row;
        for (double init_vx = -vx_max; init_vx < vx_max;
             init_vx += vx_max / N) {
            /**
             * Approach: assume homogeneous background
             * at each pixel, make small variation of outgoing light
             * intensity \propto angle
             * */
            vector<SolverRK*> solvers;
            solvers.push_back(solve(0., init_vx - d, init_vy));
            solvers.push_back(solve(0., init_vx + d, init_vy));
            solvers.push_back(solve(0., init_vx, init_vy - d));
            solvers.push_back(solve(0., init_vx, init_vy + d));
            if (no_null(solvers))
                row.push_back(intensity(
                    solvers, d / sqrt(pow(init_vx, 2) + pow(init_vy, 2))));
            else
                row.push_back(0.);
        }
        img.push_back(row);
    }

    char s[100];
    sprintf(s, "./../data/image");

    ofstream file;
    file.open(s);
    file.clear();
    for (vector<double> v : img) {
        for (double p : v) {
            file << p << ",";
        }
        file << endl;
    }

    file.close();

    return 0;
}