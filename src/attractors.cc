#include <iostream>

#include "attractor_functions.h"
#include "curve_generator.h"
#include "triangulator.h"

#include "util/system.h"

int
main (int /*argc*/, char** /*argv*/)
{
  util::system::register_segfault_handler();
  util::system::print_build_timestamp("Strange Attractors Visualization");

  // Generate the curve sampling.
  CurveGenerator::Options generator_opts;
  generator_opts.generator = AttractorFunctions::aizawa;
  generator_opts.delta = 0.01;
  generator_opts.iterations = 20000;
  CurveGenerator generator(generator_opts);
  generator.generate();
  generator.scale_and_center();
  generator.simplify();
  generator.save_ply("/tmp/curve2.ply");

  // Convert curve to 'float' accuracy.
  std::vector<math::Vec3f> curve;
  generator.get_float_curve(&curve);
  generator.clear_curve();

  // Tessellate the curve.
  Triangulator::Options triangulator_opts;
  Triangulator triangulator(triangulator_opts);
  triangulator.triangulate(curve);
  triangulator.save_mesh("/tmp/mesh2.ply");

  return 0;
}
