#include "curve_generator.h"

#include <cstdlib>
#include <iostream>

#include "math/algo.h"
#include "math/defines.h"
#include "mve/mesh_io_ply.h"
#include "mve/mesh.h"

CurveGenerator::CurveGenerator(const Options& options) {
  options_ = options;
}

void
CurveGenerator::generate() {
  std::cout << "Generating curve with " << options_.iterations
      << " samples" << std::endl;

  curve_.clear();
  curve_.resize(options_.iterations);

  Point point = options_.start;
  for (std::size_t i = 0; i < options_.iterations; ++i) {
    point = options_.generator(point, options_.delta);
    if (math::isnan(point)) {
      std::cerr << "NAN values in iteration " << i << std::endl;
      std::cerr << "  Wrong starting value? " << options_.start << std::endl;
      std::cerr << "  Small/large delta? " << options_.delta << std::endl;
      std::exit(1);
    }
    curve_[i] = point;
  }

}

void
CurveGenerator::scale_and_center() {
  if (curve_.size() < 2) return;

  // Find AABB of the curve.
  Point min(std::numeric_limits<double>::max());
  Point max(-std::numeric_limits<double>::max());
  for (std::size_t i = 0; i < curve_.size(); ++i) {
    for (std::size_t j = 0; j < 3; ++j) {
        min[j] = std::min(min[j], curve_[i][j]);
        max[j] = std::max(max[j], curve_[i][j]);
    }
  }

  // Scale and center in einheits cube.
  Point move = (min + max) / 2.0;
  Point aabb = max - min;
  double scale = aabb.maximum();
  for (std::size_t i = 0; i < curve_.size(); ++i)
    curve_[i] = (curve_[i] - move) / scale;
}

void
CurveGenerator::simplify() {
  const double angle_threshold = options_.angle_threshold;
  const double squared_min_distance = MATH_POW2(options_.min_distance);
  const double squared_max_distance = MATH_POW2(options_.max_distance);

  std::vector<bool> delete_list(curve_.size(), false);
  std::size_t angle_deletions = 0;
  std::size_t distance_deletions = 0;
  std::size_t distance_preservations = 0;

  std::size_t i0 = 0, i1 = 1, i2 = 2;
  while (i2 < curve_.size()) {
    // Delete too short distances, preserve too long distances.
    Point d0 = curve_[i1] - curve_[i0];
    if (d0.square_norm() < squared_min_distance) {
      delete_list[i1] = true;
      i1 = i2;
      i2 += 1;
      distance_deletions += 1;
      continue;
    }
    if (d0.square_norm() > squared_max_distance) {
      i0 = i1;
      i1 = i2;
      i2 += 1;
      distance_preservations += 1;
      continue;
    }

    // Delete too small angles (adaptive behavior).
    Point d1 = curve_[i2] - curve_[i1];
    const double angle = std::acos(d0.normalized().dot(d1.normalized()));
    if (angle < angle_threshold) {
      delete_list[i1] = true;
      i1 = i2;
      i2 += 1;
      angle_deletions += 1;
      continue;
    }
    i0 = i1;
    i1 = i2;
    i2 += 1;
  }

  math::algo::vector_clean(delete_list, &curve_);

  std::cout << "Simplified curve to " << curve_.size()
      << " samples" << std::endl;
  std::cout << "  Deleted " << distance_deletions
      << " short distances" << std::endl;
  std::cout << "  Deleted " << angle_deletions
      << " small angles" << std::endl;
  std::cout << "  Protected " << distance_preservations
      << " samples" << std::endl;
}

void
CurveGenerator::save_ply(const std::string& filename) {
  mve::TriangleMesh::Ptr mesh = mve::TriangleMesh::create();
  mve::TriangleMesh::VertexList& verts = mesh->get_vertices();
  for (std::size_t i = 0; i < curve_.size(); ++i) {
    verts.push_back(curve_[i]);
  }
  mve::geom::SavePLYOptions ply_opts;
  mve::geom::save_ply_mesh(mesh, filename, ply_opts);
}

void
CurveGenerator::clear_curve() {
  curve_.clear();
}

const CurveGenerator::Curve&
CurveGenerator::get_curve() const {
  return curve_;
}

void
CurveGenerator::get_float_curve(std::vector<math::Vec3f>* curve) const {
  curve->reserve(curve_.size());
  for (std::size_t i = 0; i < curve_.size(); ++i) {
    curve->push_back(curve_[i]);
  }
}
