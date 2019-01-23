#include "stdafx.h"
#include "tesselate.h"
#include "ear_clip_triangulate.h"

namespace TesselateNew {
// Curve subdivision error tolerance.
static float	s_tolerance = 1.0f;
static MeshAccepter*	s_accepter = NULL;

class PathPart{
public:
  PathPart():leftStyle(-1),
			rightStyle(-1),
			lineStyle(-1),
			closed(false),
			processed(false)
  {
  }

  int leftStyle;
  int rightStyle;
  int lineStyle;
  bool closed;
  bool processed;

  array<Point> verts;
};

static array<PathPart>	s_path_parts;
static Point	s_last_point;

void	beginShape(MeshAccepter* accepter, float curve_error_tolerance)
{
  ASSERT(accepter);
  ASSERT(s_accepter == NULL);
  s_accepter = accepter;

  // ensure we're not already in a shape or path.
  // make sure our shape state is cleared out.
  ASSERT(s_path_parts.size() == 0);

  ASSERT(curve_error_tolerance > 0);
  if (curve_error_tolerance > 0)
  {
    s_tolerance = curve_error_tolerance;
  }
  else
  {
    s_tolerance = 1.0f;
  }
}

// Return true if we did any work.
bool tryToCombinePath(int index)
{
  PathPart* pp = &s_path_parts[index];
  if (pp->closed || pp->rightStyle == -1 || pp->verts.size() <= 0) {
    return false;
  }

  if (pp->verts[0] == pp->verts.back()) {
    pp->closed = true;
    return true;
  }

  // Look for another unclosed path of the same style,
  // which could join our begin or end point.
  int style = pp->rightStyle;
  for (int i = 0; i < s_path_parts.size(); i++) {
    if (i == index) {
      continue;
    }

    PathPart* po = &s_path_parts[i];
    if (!po->closed && po->rightStyle == style && po->verts.size() > 0) {
      // Can we join?
      if (po->verts[0] == pp->verts.back()) {
        // Yes, po can be appended to pp.
        for (int j = 1; j < po->verts.size(); j++) {
          pp->verts.push_back(po->verts[j]);
        }
        po->rightStyle = -1;
        return true;
      } else if (po->verts.back() == pp->verts[0]) {
        // Yes, pp can be appended to po.
        for (int j = 1; j < pp->verts.size(); j++) {
          po->verts.push_back(pp->verts[j]);
        }
        pp->rightStyle = -1;
        return true;
      }
    }
  }

  return false;
}

void copyPointsIntoArray(array<float>* out, const array<Point>& in)
{
  if (in.size() > 0) {
    // Don't copy the end point, which should be a
    // dupe of the begin point, since the
    // triangulator does that implicitly.
    ASSERT(in[0] == in[in.size() - 1]);
    out->resize((in.size() - 1) * 2);
    for (int i = 0; i < in.size() - 1; i++) {
      (*out)[i * 2] = in[i].x;
      (*out)[i * 2 + 1] = in[i].y;
    }
  }
}

void	endShape()
{
  // TODO: there's a ton of gratuitous array copying in
  // here! Fix it by being smarter, and by better
  // abstracting the I/O methods for the triangulator.
		
  // Convert left-fill paths into new right-fill paths,
  // so we only have to deal with right-fill below.
  for (int i = 0, n = s_path_parts.size(); i < n; i++) {
    int lstyle = s_path_parts[i].leftStyle;
    int rstyle = s_path_parts[i].rightStyle;

    if (lstyle >= 0)
    {
      if (rstyle == -1)
      {
        s_path_parts[i].rightStyle = s_path_parts[i].leftStyle;
        s_path_parts[i].leftStyle = -1;
        int n = s_path_parts[i].verts.size();
        for (int j = 0, k = n >> 1; j < k; j++)
        {
          Util::swap(&s_path_parts[i].verts[j], &s_path_parts[i].verts[n - j - 1]);
        }
      }
      else
      {
        // Move the data into a new
        // proxy right path.
        s_path_parts.resize(s_path_parts.size() + 1);
        PathPart* pold = &s_path_parts[i];
        PathPart* pnew = &s_path_parts.back();

        // Copy path, in reverse, into a new right-fill path_part.
        pnew->rightStyle = lstyle;
        pnew->verts.reserve(pold->verts.size());
        for (int k = pold->verts.size() - 1; k >= 0; k--) {
          pnew->verts.push_back(pold->verts[k]);
        }
        pold->leftStyle = -1;
      }
    }
  }

  // Join path_parts together into closed paths.
  for (;;) {
    bool did_work = false;
    for (int i = 0; i < s_path_parts.size(); i++) {
      did_work = did_work || tryToCombinePath(i);
    }
    if (did_work == false) {
      break;
    }
  }
		
  // Triangulate and emit.
  for (int i = 0; i < s_path_parts.size(); i++) {
    PathPart* pp = &s_path_parts[i];
    if (!pp->processed && pp->rightStyle != -1 && pp->closed && pp->verts.size() > 0) {
      pp->processed = true;
      int style = pp->rightStyle;
      array<array<float> > paths;
      paths.resize(paths.size() + 1);
      // TODO fix gratuitous array copying
      copyPointsIntoArray(&paths.back(), pp->verts);
      // Grab all the path parts.
      for (int j = i + 1; j < s_path_parts.size(); j++) {
        PathPart* pj = &s_path_parts[j];
        if (!pj->processed
            && pj->rightStyle == style
            && pj->closed
            && pj->verts.size() > 0) {
          paths.resize(paths.size() + 1);
          copyPointsIntoArray(&paths.back(), pj->verts);
          pj->processed = true;
        }
      }
      array<float> trilist;
      EarClipTriangulate::compute(&trilist, paths.size(), &paths[0]);
      // TODO: get the results from the
      // triangulator in a more incremental
      // way (without the intermediate
      // trilist buffer!)

      // Give the results to the accepter.
      if (trilist.size() > 0) {
        s_accepter->beginTrilist(style, trilist.size() / 6);
        s_accepter->acceptTrilistBatch(
						reinterpret_cast<Point*>(&trilist[0]), trilist.size() / 2);
        s_accepter->endTrilist();
      }

      // Useful for debugging.  TODO: make a cleaner interface to this.
      // 				//xxxxxxxx
      // 				if (gameswf_tesselate_dump_shape) {
      // 					// Dump paths as a BDM file.
      // 					printf("# shape\n");
      // 					for (int i = 0; i < paths.size(); i++) {
      // 						printf("%d\n", paths[i].size() / 2);
      // 						for (j = 0; j < paths[i].size(); j += 2) {
      // 							printf("%f %f\n", paths[i][j], paths[i][j + 1]);
      // 						}
      // 					}
      // 				}
      // 				//xxxxxxxxx
    }
  }

  s_accepter->endShape();
  s_accepter = NULL;
  s_path_parts.resize(0);
}

// This call begins recording a sequence of segments, which
// all share the same fill & line styles.  Add segments to the
// shape using add_curve_segment() or add_line_segment(), and
// call end_path() when you're done with this sequence.
//
// Pass in -1 for styles that you want to disable.  Otherwise pass in
// the integral ID of the style for filling, to the left or right.
void	beginPath(int style_left, int style_right, int line_style, float ax, float ay)
{
  s_path_parts.resize(s_path_parts.size() + 1);
  s_path_parts.back().leftStyle = style_left;
  s_path_parts.back().rightStyle = style_right;
  s_path_parts.back().lineStyle = line_style;

  s_last_point.x = ax;
  s_last_point.y = ay;

  s_path_parts.back().verts.push_back(s_last_point);
}

// Add a line running from the previous anchor point to the
// given new anchor point.
void	addLineSegment(float ax, float ay)
{
  s_last_point.x = ax;
  s_last_point.y = ay;
  s_path_parts.back().verts.push_back(s_last_point);
}

// Recursive routine to generate bezier curve within tolerance.
static void	curve(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y)
{
#ifndef NDEBUG
  static int	recursion_count = 0;
  recursion_count++;
  if (recursion_count > 500)
  {
    ASSERT(0);	// probably a bug!
  }
#endif // not NDEBUG

  // TODO: use struct point in here?

  // Midpoint on line between two endpoints.
  float	midx = (p0x + p2x) * 0.5f;
  float	midy = (p0y + p2y) * 0.5f;

  // Midpoint on the curve.
  float	qx = (midx + p1x) * 0.5f;
  float	qy = (midy + p1y) * 0.5f;

  float	dist = fabsf(midx - qx) + fabsf(midy - qy);

  if (dist < s_tolerance)
  {
    // Emit edge.
    addLineSegment(p2x, p2y);
  }
  else
  {
    // Error is too large; subdivide.
    curve(p0x, p0y, (p0x + p1x) * 0.5f, (p0y + p1y) * 0.5f, qx, qy);
    curve(qx, qy, (p1x + p2x) * 0.5f, (p1y + p2y) * 0.5f, p2x, p2y);
  }

#ifndef NDEBUG
  recursion_count--;
#endif // not NDEBUG
}

// Add a curve segment to the shape.  The curve segment is a
// quadratic bezier, running from the previous anchor point to
// the given new anchor point (ax, ay), with (cx, cy) acting
// as the control point in between.
void	addCurveSegment(float cx, float cy, float ax, float ay)
{
  if (cx == ax && cy == ay) {
    // Early-out for degenerate straight segments.
    addLineSegment(ax, ay);
  } else {
    // Subdivide, and add line segments...
    curve(s_last_point.x, s_last_point.y, cx, cy, ax, ay);
  }
}

// Mark the end of a set of edges that all use the same styles.
void	endPath()
{
  if (s_path_parts.back().lineStyle >= 0 && s_path_parts.back().verts.size() > 1) {
    // Emit our line.
    s_accepter->acceptLineStrip(
				s_path_parts.back().lineStyle,
				&s_path_parts.back().verts[0],
				s_path_parts.back().verts.size());
  }
}
};
