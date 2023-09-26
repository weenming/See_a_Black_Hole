#include <iostream>
#include <vector>

#include "./../utils/const.hpp"
#include "./../utils/data3d.hpp"
#include "black_hole.hpp"



class SolverRK {
 public:
  SolverRK(BlackHole *b, VectorBL *init_r, VectorCartesian *init_v) {
    this->black_hole = b;
    this->init_r = init_r;
    this->init_v = init_v;
  }

  void solve(int max_step, double step_size) {
 
    int i = 0;

    initialize();  // calc. L, K, pr0, ptheta0

    // intial \vec{y}
    double r = init_r->r;
    double theta = init_r->theta;
    double phi = init_r->phi;
    double pr = pr0, ptheta = ptheta0;

    this->trajectory = new vector<VectorBL>(max_step);
    // use RK-4 to solve, save trajectory to trajectory
    double last_pr = pr;
    while (i < max_step) {
      double k1_r = dr(r, theta, pr, ptheta);
      double k1_theta = dtheta(r, theta, pr, ptheta);
      double k1_phi = dphi(r, theta, pr, ptheta);
      double k1_pr = dpr(r, theta, pr, ptheta);
      double k1_ptheta = dptheta(r, theta, pr, ptheta);

      double r_tmp = r + k1_r * step_size / 2;
      double theta_tmp = theta + k1_theta * step_size / 2;
      double phi_tmp = phi + k1_phi * step_size / 2;
      double pr_tmp = pr + k1_pr * step_size / 2;
      double ptheta_tmp = ptheta + k1_ptheta * step_size / 2;

      double k2_r = dr(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k2_theta = dtheta(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k2_phi = dphi(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k2_pr = dpr(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k2_ptheta = dptheta(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);

      r_tmp = r + k2_r * step_size / 2;
      theta_tmp = theta + k2_theta * step_size / 2;
      phi_tmp = phi + k2_phi * step_size / 2;
      pr_tmp = pr + k2_pr * step_size / 2;
      ptheta_tmp = ptheta + k2_ptheta * step_size / 2;

      double k3_r = dr(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k3_theta = dtheta(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k3_phi = dphi(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k3_pr = dpr(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k3_ptheta = dptheta(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);

      r_tmp = r + k3_r * step_size;
      theta_tmp = theta + k3_theta * step_size;
      phi_tmp = phi + k3_phi * step_size;
      pr_tmp = pr + k3_pr * step_size;
      ptheta_tmp = ptheta + k3_ptheta * step_size;

      double k4_r = dr(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k4_theta = dtheta(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k4_phi = dphi(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k4_pr = dpr(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);
      double k4_ptheta = dptheta(r_tmp, theta_tmp, pr_tmp, ptheta_tmp);

      r += RK_k(k1_r, k2_r, k3_r, k4_r) * step_size;
      theta += RK_k(k1_theta, k2_theta, k3_theta, k4_theta) * step_size;
      phi += RK_k(k1_phi, k2_phi, k3_phi, k4_phi) * step_size;
      pr += RK_k(k1_pr, k2_pr, k3_pr, k4_pr) * step_size;
      ptheta += RK_k(k1_ptheta, k2_ptheta, k3_ptheta, k4_ptheta) * step_size;

      // check
      if (this->is_inside(r, pr, last_pr)) {
        final_r = final_v = NULL;
        return;
      }
      last_pr = pr;

      trajectory->at(i) = (VectorBL(vector3({r, theta, phi}), black_hole->a));
      i++;
    }
    trajectory->shrink_to_fit();

    final_r = new VectorBL(vector3{r, theta, phi}, black_hole->a);
    final_v = new VectorBL(
        trajectory->at(i - 1).get_x() - trajectory->at(i - 2).get_x(),
        trajectory->at(i - 1).get_y() - trajectory->at(i - 2).get_y(),
        trajectory->at(i - 1).get_z() - trajectory->at(i - 2).get_z(),
        black_hole->a);
    return;
  }

  VectorBL *init_r;  // in the BH frame
  VectorCartesian
      *init_v;  // in the camera frame (x-axis is radius in BH frame)

  vector<VectorBL> *trajectory;
  VectorBL *final_r;
  VectorBL *final_v;

  BlackHole *black_hole;
  const double INF_R = 100 * 2;  // M is set to 1 so R_s is 2
  bool is_inf(double r) { return r > INF_R; }
  bool is_inside(double r, double pr, double last_pr) {
    return r < 2 || abs(pr) > 1e5 * abs(last_pr);
  }

 private:
  // consts
  double L, K;
  // init val
  double pr0, ptheta0;

  // helpers
  double Delta(double r) {
    return pow(r, 2) + pow(this->black_hole->a, 2) - 2 * r;
  }

  double Sigma(double &r, double &theta) {
    return pow(r, 2) + pow(this->black_hole->a, 2) * pow(cos(theta), 2);
  }

  // RK step
  double dr(double &r, double &theta, double &pr, double &ptheta) {
    return pr * Delta(r) / Sigma(r, theta);
  }
  double dtheta(double &r, double &theta, double &pr, double &ptheta) {
    return ptheta / Sigma(r, theta);
  }
  double dphi(double &r, double &theta, double &pr, double &ptheta) {
    return (2 * this->black_hole->a * r +
            (Sigma(r, theta) - 2 * r) * L / pow(sin(theta), 2)) /
           (Delta(r) * Sigma(r, theta));
  }
  double dpr(double &r, double &theta, double &pr, double &ptheta) {
    return ((r - 1) * (-K) + 2 * r * (pow(r, 2) + pow(this->black_hole->a, 2)) -
            2 * this->black_hole->a * L) /
               (Delta(r) * Sigma(r, theta)) -
           (2 * pow(pr, 2) * (r - 1)) / Sigma(r, theta);
  }
  double dptheta(double &r, double &theta, double &pr, double &ptheta) {
    return (sin(theta) * cos(theta) *
            (pow(L, 2) / pow(sin(theta), 4) - pow(black_hole->a, 2))) /
           Sigma(r, theta);
  }

  // RK avg
  double RK_k(double k1, double k2, double k3, double k4) {
    return (k1 + 2 * k2 + 2 * k3 + k4) / 6;
  }

  void initialize() {
    // note that x, y corresponds to the image coordinate
    double r0dot = init_v->z / init_v->get_r();
    double theta0dot = init_v->y / (init_v->get_r() * init_r->r);
    double phi0dot =
        init_v->x / (init_v->get_r() * init_r->r * sin(init_r->theta));

    double s0 = Sigma(init_r->r, init_r->theta) - 2 * init_r->r;
    double e0 =
        sqrt(s0 * (pow(r0dot, 2) / Delta(init_r->r) + pow(theta0dot, 2)) +
             Delta(init_r->r) * pow(sin(init_r->theta), 2) * pow(phi0dot, 2));

    this->L = (Sigma(init_r->r, init_r->theta) * Delta(init_r->r) * phi0dot -
               2 * black_hole->a * init_r->r * e0) *
              pow(sin(init_r->theta), 2) / (s0 * e0);  // const: L_z
    this->K = pow((theta0dot * Sigma(init_r->r, init_r->theta)), 2) +
              pow(black_hole->a * sin(init_r->theta), 2) +
              pow(L, 2) / pow(sin(init_r->theta), 2);  // const: L_{\theta}

    this->pr0 =
        (r0dot * Sigma(init_r->r, init_r->theta)) / (Delta(init_r->r) * e0);
    this->ptheta0 = Sigma(init_r->r, init_r->theta) * theta0dot / e0;
  }
};