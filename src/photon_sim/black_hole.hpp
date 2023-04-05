#include <cmath>
#include <vector>

#include "./../utils/const.hpp"

using namespace std;

class BlackHole {
 public:
  double M;
  double J;
  // no charge

  double R_s;
  double a;

  BlackHole(double m, double j) {
    M = m;
    J = j;
    R_s = 2 * G * M / pow(c, 2);
    a = J / (M * c);
  }
};