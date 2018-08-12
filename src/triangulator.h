#ifndef ATTRACTORS_TRIANGULATOR_HEADER
#define ATTRACTORS_TRIANGULATOR_HEADER

#include <string>
#include <vector>

#include "math/vector.h"
#include "mve/mesh.h"

class Triangulator {
 public:
  struct Options {
    float thickness = 0.01f;
    bool add_caps = true;

    math::Vec3f lcs_init = math::Vec3f(1.0f, 2.0f, 3.0f);
    float twist_factor = 0.0f;
  };

 public:
  Triangulator(const Options& options);
  void triangulate(const std::vector<math::Vec3f>& curve);
  mve::TriangleMesh::Ptr get_mesh() const;
  void save_mesh(const std::string& filename);

 private:
  void compute_lcs(const std::vector<math::Vec3f>& curve);
  void create_tube_profile(const math::Vec3f& pos,
      const math::Vec3f& x, const math::Vec3f& y,
      mve::TriangleMesh::VertexList* verts);
  void connect_tube_segments(
      const mve::TriangleMesh::VertexList& vertices,
      std::size_t verts_per_segment,
      mve::TriangleMesh::FaceList* faces);
  void triangulate_caps(std::size_t verts_per_segment, bool tube_start);

 private:
  Options options_;
  mve::TriangleMesh::Ptr mesh_;
  std::vector<math::Vec3f> lcs_;
};

#endif /* ATTRACTORS_TRIANGULATOR_HEADER */
