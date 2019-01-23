#pragma once

namespace TesselateNew {
class MeshAccepter {
public:
  virtual ~MeshAccepter() {}
  // Caller begins a trilist of a particular
  // style, then passes in batches of individual
  // triangles, then ends the trilist.  The idea
  // behind small batches is that the data can
  // pass directly from the tesselator into the
  // output mesh storage, without allocating a
  // big intermediate buffer.
  virtual void beginTrilist(int style, int expected_triangle_count) = 0;
  virtual void acceptTrilistBatch(const Point trilist[], int point_count) = 0;
  virtual void endTrilist() = 0;

  virtual void acceptLineStrip(int style, const Point coords[], int point_count) = 0;
  virtual void endShape() = 0;
};

void	beginShape(MeshAccepter* accepter, float curve_error_tolerance);
void	endShape();

// A path is a subpart of a shape, having a consistent style.
void	beginPath(int style_left, int style_right, int line_style, float ax, float ay);
void	addLineSegment(float ax, float ay);
void	addCurveSegment(float cx, float cy, float ax, float ay);
void	endPath();
}; // end namespace tesselate_new
