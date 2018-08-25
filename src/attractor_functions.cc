#include "attractor_functions.h"

math::Vec3d
AttractorFunctions::aizawa(const math::Vec3d& v, double dt) {
  if (dt == 0.0) {
    return math::Vec3d(0.1, 0.0, 0.0);
  }

  const double a = 0.95;
  const double b = 0.7;
  const double c = 0.6;
  const double d = 3.5;
  const double e = 0.25;
  const double f = 0.1;

  math::Vec3d v1;
  v1[0] = v[0] + dt * ((v[2] - b) * v[0] - d * v[1]);
  v1[1] = v[1] + dt * (d * v[0] + (v[2] - b) * v[1]);
  v1[2] = v[2] + dt * (c
      + a * v[2]
      - MATH_POW3(v[2]) / 3.0
      - (MATH_POW2(v[0]) + MATH_POW2(v[1])) * (1.0 + e * v[2])
      + f * v[2] * MATH_POW3(v[0]));
  return v1;
}

math::Vec3d
AttractorFunctions::bouali(const math::Vec3d& v, double dt) {
  if (dt == 0.0) {
    return math::Vec3d(1.0, 0.1, 0.1);
  }

  const double a = 0.3;
  const double s = 1.0;

  math::Vec3d v1;
  v1[0] = v[0] + dt * (v[0] * (4.0 - v[1]) + a * v[2]);
  v1[1] = v[1] + dt * (-v[1] * (1.0 - MATH_POW2(v[0])));
  v1[2] = v[2] + dt * (-v[0] * (1.5 - s * v[2]) - 0.05 * v[2]);
  return v1;
}

math::Vec3d
AttractorFunctions::burke_shaw(const math::Vec3d& v, double dt) {
  if (dt == 0.0) {
    return math::Vec3d(1.0, 0.0, 0.0);
  }

  const double s = 10.0;
  const double t = 4.272;

  math::Vec3d v1;
  v1[0] = v[0] + dt * (-s * (v[0] + v[1]));
  v1[1] = v[1] + dt * (-v[1] - s * v[0] * v[2]);
  v1[2] = v[2] + dt * (s * v[0] * v[1] + t);
  return v1;
}

math::Vec3d
AttractorFunctions::chen_celikovsky(const math::Vec3d& v, double dt) {
  if (dt == 0.0) {
    return math::Vec3d(1.0, 1.0, 1.0);
  }

  const double a = 36.0;
  const double b = 3.0;
  const double c = 20.0;

  math::Vec3d v1;
  v1[0] = v[0] + dt * (a * (v[1] - v[0]));
  v1[1] = v[1] + dt * (-v[0] * v[2] + c * v[1]);
  v1[2] = v[2] + dt * (v[0] * v[1] - b * v[2]);
  return v1;
}

math::Vec3d
AttractorFunctions::chen_lee(const math::Vec3d& v, double dt) {
  if (dt == 0.0) {
    return math::Vec3d(1.0, 0.0, 4.5);
  }

  const double a = 5.0;
  const double b = -10.0;
  const double c = -0.38;

  math::Vec3d v1;
  v1[0] = v[0] + dt * (a * v[0] - v[1] * v[2]);
  v1[1] = v[1] + dt * (b * v[1] + v[0] * v[2]);
  v1[2] = v[2] + dt * (c * v[2] + v[0] * v[1] / 3.0);
  return v1;
}

math::Vec3d
AttractorFunctions::dequan_li(const math::Vec3d& v, double dt) {
  if (dt == 0.0) {
    return math::Vec3d(0.349, 0.0, -0.16);
  }

  const double a = 40.0;
  const double c = 1.833;
  const double d = 0.16;
  const double e = 0.65;
  const double k = 55.0;
  const double f = 20.0;

  math::Vec3d v1;
  v1[0] = v[0] + dt * (a * (v[1] - v[0]) + d * v[0] * v[2]);
  v1[1] = v[1] + dt * (k * v[0] + f * v[1] - v[0] * v[2]);
  v1[2] = v[2] + dt * (c * v[2] + v[0] * v[1] - e * v[0] * v[0]);
  return v1;
}

AttractorFunctions::Type
AttractorFunctions::for_string(const std::string& name) {
  if (name == "aizawa") return aizawa;
  if (name == "bouali") return bouali;
  if (name == "burke-shaw") return burke_shaw;
  if (name == "chen-celikovsky") return chen_celikovsky;
  if (name == "chen-lee") return chen_lee;
  if (name == "dequan-li") return dequan_li;

  return nullptr;
}
