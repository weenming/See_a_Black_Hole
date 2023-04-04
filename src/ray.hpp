#include <vector>

#include "data3d.hpp"

using namespace std;

class Ray {
 public:
  VectorCartesian* position;
  VectorCartesian* velocity;
  Ray(vector3 initial_pos, vector3 initial_vel) {
    position = new VectorCartesian(initial_pos);
    velocity = new VectorCartesian(initial_vel);
  }
};
