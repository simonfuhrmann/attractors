#ifndef ATTRACTORS_ATTRACTOR_FUNCTIONS_HEADER
#define ATTRACTORS_ATTRACTOR_FUNCTIONS_HEADER

#include "math/vector.h"

class AttractorFunctions {
 public:
  // Aizawa attraktor function
  // http://www.3d-meier.de/tut19/Seite3.html
  static math::Vec3d aizawa(const math::Vec3d& vn, double dt);
};

#endif /* ATTRACTORS_ATTRACTOR_FUNCTIONS_HEADER */
