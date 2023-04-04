#include <stdio.h>

#include <vector>

#include "black_hole.hpp"
#include "const.hpp"
#include "data3d.hpp"
#include "ray.hpp"

/**
 * For light around a spinning BH, one can use Kerr metric to describe it.
 *
 *   c^2 d\tau^2
 * =  (1 - {R_s r \over \Sigma} c^2 dt^2)
 *  - {\Sigma \over \Delta} dr^2
 *  - \Sigma d\theta^2
 *  -(r^2 + a^2 + {R_s r a^2 \sin{\theta}^2} \over \Sigma)\sin{\theta}^2 d\phi^2
 *  + {2 R_s r a \sin^{\theta}^2 \over \Sigma} c dt d\phi
 *
 * where
 * a = {J \over Mc}, R_s = 2GM / c^2
 * \Sigma = r^2 + a^2 \cos{\theta}^2,
 * \Delta = r^2 - R_s r + a^2
 *
 * Note that for photons, d\tau = 0
 *
 * An orbit in the xy plane would be simplified to
 * 0 =
 *   (1 - {R_s r \over \Sigma} c^2 dt^2)
 *  - {\Sigma \over \Delta} dr^2
 *
 * where
 * a = {J \over Mc},
 * \Sigma = r^2 + a^2,
 * \Delta = r^2 - R_s r + a^2
 */

class BaseSolver {
 public:
  VectorSpherical init_r;
  VectorSpherical init_v;

  vector<VectorSpherical> trajectory;
  VectorSpherical final_r;
  VectorSpherical final_v;
  virtual void solve() = 0;

 protected:
  BlackHole black_hole;
  const double INF_R = 100 * black_hole.R_s;
  bool is_inf(VectorSpherical vec) { return vec.r > INF_R; }
  bool is_inside(VectorSpherical vec) { return vec.r < black_hole.R_s; }
};

class Solver2d : BaseSolver {
 public:
  void solve() {
    // use RK-4 to solve, save trajectory to trajectory

    return;
  }
};