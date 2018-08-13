#include "triangulator.h"

#include "math/defines.h"
#include "mve/mesh_io_ply.h"

Triangulator::Triangulator(const Options& options) {
  options_ = options;
}

void
Triangulator::triangulate(const std::vector<math::Vec3f>& curve) {
  mesh_ = mve::TriangleMesh::create();
  if (curve.size() < 2) return;

  mve::TriangleMesh::VertexList& mv = mesh_->get_vertices();
  mve::TriangleMesh::FaceList& mf = mesh_->get_faces();

  // Assign a local coordiante system to each vertex.
  compute_lcs(curve);

  // Create a triangulated tube.
  for (std::size_t i = 0; i < curve.size(); ++i) {
    // Create N vertices per curve vertex in the local coordinate system.
    const math::Vec3f d = (i == 0
        ? curve[i + 1] - curve[i]
        : curve[i] - curve[i - 1]);
    const math::Vec3f x = lcs_[i];
    const math::Vec3f y = x.cross(d).normalize();
    create_tube_profile(curve[i], x, y, &mv);
  }

  // Triangulate the tube by connecting neighboring segments.
  const std::size_t verts_per_segment = mv.size() / curve.size();
  connect_tube_segments(mv, verts_per_segment, &mf);

  // Add caps to the tube. Note: Must start with end cap!
  if (options_.add_caps) {
    triangulate_caps(verts_per_segment, false);
    triangulate_caps(verts_per_segment, true);
  }
}

mve::TriangleMesh::Ptr
Triangulator::get_mesh() const {
  return mesh_;
}

void
Triangulator::save_mesh(const std::string& filename) {
  mve::geom::SavePLYOptions ply_opts;
  mve::geom::save_ply_mesh(mesh_, filename, ply_opts);
}

void
Triangulator::compute_lcs(const std::vector<math::Vec3f>& curve) {
  lcs_.clear();
  lcs_.resize(curve.size());

  // Initialize with the previous LCS and make it orthogonal.
  for (std::size_t i = 0; i < curve.size(); ++i) {
    const math::Vec3f x = (i == 0
        ? options_.lcs_init
        : lcs_[i - 1]);
    const math::Vec3f d = (i + 1 == curve.size()
        ? curve[i] - curve[i - 1]
        : curve[i + 1] - curve[i]);
    lcs_[i] = d.cross(x.cross(d)).normalized();
  }
}

void
Triangulator::create_tube_profile(const math::Vec3f& pos,
    const math::Vec3f& x,
    const math::Vec3f& y,
    mve::TriangleMesh::VertexList* verts) {
  const float thickness = options_.thickness;

#if 1
  for (int i = 0; i < 20; ++i) {
    float xf = std::cos(static_cast<float>(i) / 20.0f * MATH_TAU);
    float yf = std::sin(static_cast<float>(i) / 20.0f * MATH_TAU);
    float tf = thickness * ((i/2) % 2 ? 0.65f : 1.0f);
    verts->push_back(pos + tf * (x * xf + y * yf));
    verts->push_back(pos + tf * (x * xf + y * yf));
  }
#endif

#if 0
  for (int i = 0; i < 20; ++i) {
    float xf = std::cos(static_cast<float>(i) / 20.0f * MATH_TAU);
    float yf = std::sin(static_cast<float>(i) / 20.0f * MATH_TAU);
    float tf = thickness;
    verts->push_back(pos + tf * (x * xf + y * yf));
  }
#endif
}

void
Triangulator::connect_tube_segments(
    const mve::TriangleMesh::VertexList& vertices,
    std::size_t verts_per_segment,
    mve::TriangleMesh::FaceList* faces) {
  for (std::size_t s = 0; s < vertices.size(); s += verts_per_segment) {
    // Each segment is connected to the previous one. Skip first one.
    if (s == 0) continue;

    // Start indices for the last two segments.
    std::size_t n1 = s - verts_per_segment;
    std::size_t n2 = s;

    for (std::size_t i = 0; i < verts_per_segment; ++i) {
      // Skip quad if the vertex is duplicated. Prevents degenerated faces.
      std::size_t ip1 = (i + 1) % verts_per_segment;
      if (vertices[n1 + i] == vertices[n1 + ip1]) continue;

      // Connect a quad of vertices with 2 triangles.
      faces->push_back(n1 + i);
      faces->push_back(n2 + i);
      faces->push_back(n1 + ip1);
      faces->push_back(n1 + ip1);
      faces->push_back(n2 + i);
      faces->push_back(n2 + ip1);
    }
  }
}

void
Triangulator::triangulate_caps(std::size_t verts_per_segment, bool tube_start) {
  mve::TriangleMesh::VertexList& verts = mesh_->get_vertices();
  mve::TriangleMesh::FaceList& faces = mesh_->get_faces();
  const std::size_t start_vert_id = verts.size();
  const std::size_t copy_offset = tube_start ? 0 : verts.size() - verts_per_segment;

  // Duplicate the cap vertices.
  std::size_t num_cap_verts = 0;
  math::Vec3f center(0.0f);
  for (std::size_t i = 0; i < verts_per_segment; ++i) {
    const std::size_t ip1 = (i + 1) % verts_per_segment;
    if (verts[copy_offset + i] == verts[copy_offset + ip1]) continue;
    verts.push_back(verts[copy_offset + i]);
    center += verts.back();
    num_cap_verts += 1;
  }

  // Add a center vertex.
  center /= static_cast<float>(num_cap_verts);
  verts.push_back(center);

  // Triangulate. Change orientation of triangles if end cap.
  for (std::size_t i = 0; i < num_cap_verts; ++i) {
    const std::size_t ip1 = (i + 1) % num_cap_verts;
    faces.push_back(start_vert_id + num_cap_verts);
    faces.push_back(start_vert_id + (tube_start ? i : ip1));
    faces.push_back(start_vert_id + (tube_start ? ip1 : i));
  }
}
