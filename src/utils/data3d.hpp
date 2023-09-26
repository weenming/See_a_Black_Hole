
#include <cmath>
#include <vector>

#include "const.hpp"
using namespace std;

using vector3 = vector<double>;

class VectorCartesian {
 public:
  double x;
  double y;
  double z;

  VectorCartesian() { return; }
  VectorCartesian(double x, double y, double z) { set_cartesian(x, y, z); }
  VectorCartesian(vector3 vec) { set_cartesian(vec); }

  void set_cartesian(vector3 vec) {
    x = vec[0];
    y = vec[1];
    z = vec[2];
  }

  void set_cartesian(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  void set_spherical(double r, double theta, double phi) {
    x = r * sin(theta) * cos(phi);  // x = r \sin{\theta} \cos{\phi}
    y = r * sin(theta) * sin(phi);
    z = r * cos(theta);
  };
  void set_spherical(vector3 in_vec) {
    // param: in_vec = {r, theta, phi}
    double r = in_vec[0];
    double theta = in_vec[1];
    double phi = in_vec[2];
    set_spherical(r, theta, phi);
  }

  double get_r() { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }
  double get_theta() { return acos(z / get_r()); }
  double get_phi() {
    double ang = atan(y / x);
    int is_neg = y < 0;
    ang += ((ang < 0) + (is_neg < 0)) * Pi;

    return ang;
  }

  vector3 get_normalized_cartesian() {
    // normalized cartedian coord.
    double norm = get_r();
    return vector3({x / norm, y / norm, z / norm});
  };
};

class VectorSpherical {
 public:
  double r;
  double theta;
  double phi;

  VectorSpherical() { return; }
  VectorSpherical(double x, double y, double z) { set_cartesian(x, y, z); }
  VectorSpherical(vector3 vec) { set_spherical(vec); }
  VectorSpherical(VectorCartesian vec) {
    set_spherical(vec.get_r(), vec.get_theta(), vec.get_phi());
  }

  void set_spherical(vector3 vec) {
    r = vec[0];
    theta = vec[1];
    phi = vec[2];
  }
  void set_spherical(double r, double theta, double phi) {
    this->r = r;
    this->theta = theta;
    this->phi = phi;
  }
  void set_cartesian(double x, double y, double z) {
    r = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    theta = acos(z / r);

    double ang = atan(y / x);
    int is_neg = y < 0;
    ang += ((ang < 0) + (is_neg < 0)) * Pi;
    phi = ang;
  };

  double get_x() { return r * sin(theta) * cos(phi); }
  double get_y() { return r * sin(theta) * sin(phi); }
  double get_z() { return r * cos(theta); }
};

class VectorBL {
 public:
  double r;
  double theta;
  double phi;
  double a;

  VectorBL() { return; }
  VectorBL(double x, double y, double z, double a) {
    set_cartesian(x, y, z, a);
    this->a = a;
  }
  VectorBL(vector3 vec, double a) {
    vec[0] = make_BL_r(vec[0], a);
    set_BL(vec);
    this->a = a;
  }
  VectorBL(VectorCartesian vec, double a) {
    set_BL(make_BL_r(r, a), vec.get_theta(), vec.get_phi());
    this->a = a;
  }

  void set_BL(vector3 vec) {
    r = vec[0];
    theta = vec[1];
    phi = vec[2];
  }
  void set_BL(double r, double theta, double phi) {
    this->r = r;
    this->theta = theta;
    this->phi = phi;
  }
  void set_cartesian(double x, double y, double z, double a) {
    r = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    r = make_BL_r(r, a);
    theta = acos(z / r);

    double ang = atan(y / x);
    int is_neg = y < 0;
    ang += ((ang < 0) + (is_neg < 0)) * Pi;
    phi = ang;
  };

  double get_x() { return make_BL_r(r, a) * sin(theta) * cos(phi); }
  double get_y() { return make_BL_r(r, a) * sin(theta) * sin(phi); }
  double get_z() { return make_BL_r(r, a) * cos(theta); }

  double make_BL_r(double r, double a) { return sqrt(pow(r, 2) + pow(a, 2)); }
};
