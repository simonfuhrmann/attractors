#include "attractor_functions.h"

math::Vec3d
AttractorFunctions::aizawa(const math::Vec3d& vn, double dt)
{
  const double a = 0.95;
  const double b = 0.7;
  const double c = 0.6;
  const double d = 3.5;
  const double e = 0.25;
  const double f = 0.1;

  math::Vec3d vn1;
  vn1[0] = vn[0] + dt * ((vn[2] - b) * vn[0] - d * vn[1]);
  vn1[1] = vn[1] + dt * (d * vn[0] + (vn[2] - b) * vn[1]);
  vn1[2] = vn[2] + dt * (c
      + a * vn[2]
      - MATH_POW3(vn[2]) / 3.0
      - (MATH_POW2(vn[0]) + MATH_POW2(vn[1])) * (1.0 + e * vn[2])
      + f * vn[2] * MATH_POW3(vn[0]));
  return vn1;
}
