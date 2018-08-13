#ifndef ATTRACTORS_ATTRACTOR_FUNCTIONS_HEADER
#define ATTRACTORS_ATTRACTOR_FUNCTIONS_HEADER

#include "math/vector.h"

class AttractorFunctions {
 public:
  // Aizawa attractor function
  // http://www.3d-meier.de/tut19/Seite3.html
  // Start value: 0.1, 0, 0
  // Good values: 0.01 / 20000  and 0.009 / 20000
  static math::Vec3d aizawa(const math::Vec3d& v, double dt);

  // Bouali attractor function
  // Start value: 1.0, 0.1, 0.1
  // Good values: 0.006 / 40000
  static math::Vec3d bouali(const math::Vec3d& v, double dt);

};

#endif /* ATTRACTORS_ATTRACTOR_FUNCTIONS_HEADER */
