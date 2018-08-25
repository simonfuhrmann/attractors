#include <string>
#include <iostream>

#include "attractor_functions.h"
#include "curve_generator.h"
#include "triangulator.h"

#include "util/arguments.h"
#include "util/file_system.h"
#include "util/system.h"

struct AppSettings {
  std::string curve_filename;
  std::string mesh_filename;
  std::string function = "aizawa";
  double time_delta = 0.01;
  std::size_t iterations = 20000;
  bool simplify = false;
  float thickness = 0.01;
};

int
main (int argc, char** argv)
{
  util::system::register_segfault_handler();
  util::system::print_build_timestamp("Strange Attractors Visualization");

  // Parse command line arguments.
  util::Arguments args;
  args.set_usage(argv[0], "[ OPTIONS ] OUTPUT_MESH");
  args.set_exit_on_error(true);
  args.set_nonopt_maxnum(1);
  args.set_nonopt_minnum(1);
  args.set_description("A utility that samples an attractor function and "
      "outputs a triangulated version of the attractor curve.");
  args.add_option('c', "curve", true, "Save curve to file [disabled]");
  args.add_option('f', "function", true, "The attractor function [aizawa]");
  args.add_option('s', "simplify", false, "Simplify the curve [false]");
  args.add_option('d', "delta", true, "The time delta between samples [0.01]");
  args.add_option('i', "iterations", true, "The number of samples [20000]");
  args.add_option('t', "thickness", true, "The thickness of the tube [0.01]");
  args.parse(argc, argv);

  AppSettings settings;
  settings.mesh_filename = util::fs::sanitize_path(args.get_nth_nonopt(0));
  while (util::ArgResult const* arg = args.next_option()) {
    if (arg->opt->lopt == "curve") {
      settings.curve_filename = arg->arg;
    } else if (arg->opt->lopt == "function") {
      settings.function = arg->arg;
    } else if (arg->opt->lopt == "delta") {
      settings.time_delta = arg->get_arg<double>();
    } else if (arg->opt->lopt == "iterations") {
      settings.iterations = arg->get_arg<std::size_t>();
    } else if (arg->opt->lopt == "simplify") {
      settings.simplify = true;
    } else if (arg->opt->lopt == "thickness") {
      settings.thickness = arg->get_arg<float>();
    } else {
      std::cerr << "Unexpected option: " << arg->opt->lopt << std::endl;
      return 1;
    }
  }

  // Generate the curve sampling.
  CurveGenerator::Options generator_opts;
  // generator_opts.generator = AttractorFunctions::aizawa;
  generator_opts.generator = AttractorFunctions::for_string(settings.function);
  generator_opts.delta = settings.time_delta;
  generator_opts.iterations = settings.iterations;
  CurveGenerator generator(generator_opts);
  generator.generate();
  generator.scale_and_center();
  if (settings.simplify) {
    generator.simplify();
  } else {
    std::cout << "SKIPPING curve simplification (enable with -s)." << std::endl;
  }
  if (!settings.curve_filename.empty()) {
    generator.save_ply(settings.curve_filename);
  }

  // Convert curve to 'float' accuracy.
  std::vector<math::Vec3f> curve;
  generator.get_float_curve(&curve);
  generator.clear_curve();

  // Tessellate the curve.
  Triangulator::Options triangulator_opts;
  triangulator_opts.thickness = settings.thickness;
  Triangulator triangulator(triangulator_opts);
  triangulator.triangulate(curve);
  triangulator.save_mesh(settings.mesh_filename);

  return 0;
}
