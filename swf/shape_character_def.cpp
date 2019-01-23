#include "stdafx.h"

#include "shape_character_def.h"
#include "tesselate.h"
#include "movie_definition_sub.h"

Edge::Edge() :
		cx(0), cy(0),
		ax(0), ay(0)
{
}

Edge::Edge(float cx, float cy, float ax, float ay)
{
  // edges can't be infinite
  this->cx = cx >= -3.402823466e+38F && cx <= 3.402823466e+38F ? cx : 0.0f;
  this->cy = cy >= -3.402823466e+38F && cy <= 3.402823466e+38F ? cy : 0.0f;
  this->ax = ax >= -3.402823466e+38F && ax <= 3.402823466e+38F ? ax : 0.0f;
  this->ay = ay >= -3.402823466e+38F && ay <= 3.402823466e+38F ? ay : 0.0f;
}

bool	Edge::isStraight() const
{
  return cx == ax && cy == ay;
}

// Send this segment to the tesselator.
void	Edge::tesselateCurve() const
{
  TesselateNew::addCurveSegment(cx, cy, ax, ay);
}

// Send this segment to the tesselator.
void	Edge::tesselateCurveNew() const
{
  TesselateNew::addCurveSegment(cx, cy, ax, ay);
}

Path::Path():newShape(false)
{
  reset(0, 0, 0, 0, 0);
}

Path::Path(float ax, float ay, int fill0, int fill1, int line)
{
  reset(ax, ay, fill0, fill1, line);
}

// Reset all our members to the given values, and clear our edge list.
void	Path::reset(float ax, float ay, int fill0, int fill1, int line)
{
  this->ax = ax;
  this->ay = ay;
  this->fill0 = fill0;
  this->fill1 = fill1;
  this->line = line;

  edges.resize(0);

  ASSERT(isEmpty());
}

// Return true if we have no edges.
bool	Path::isEmpty() const
{
  return edges.size() == 0;
}

// Point-in-shape test.  Return true if the query point is on the filled
// interior of this shape.
bool	Path::pointTest(float x, float y)
{
  if (edges.size() <= 0)
  {
    return false;
  }

  if (fill0 < 0)
  {
    // No interior fill.
			
    // @@ This isn't quite right due to some paths
    // doing double-duty with both fill0 and fill1
    // styles.

    // TODO: get rid of this stupid fill0/fill1
    // business -- a path should always be
    // counterclockwise and have one fill.  For
    // input paths with fill1, generate a separate
    // reversed path with fill set to fill1.
    // Group all paths with the same fill into a
    // path group; do the point_test on the whole
    // group.
    return false;
  }

  // Shoot a horizontal ray from (x,y) to the right, and
  // count the number of edge crossings.  An even number
  // of crossings means the point is outside; an odd
  // number means it's inside.

  float x0 = ax;
  float y0 = ay;

  int ray_crossings = 0;
  for (int i = 0, n = edges.size(); i < n; i++)
  {
    const Edge& e = edges[i];

    float x1 = e.ax;
    float y1 = e.ay;

    if (e.isStraight()) {
      // Straight-line case.
				
      // See if (x0,y0)-(x1,y1) crosses (x,y)-(infinity,y)
			
      // Does the segment straddle the horizontal ray?
      bool cross_up = (y0 < y && y1 >= y);
      bool cross_down = (!cross_up) && (y0 > y && y1 <= y);
      if (cross_up || cross_down)
      {
        // Straddles.
				
        // Is the crossing point to the right of x?
        float dy = y1 - y0;

        // x_intercept = x0 + (x1 - x0) * (y - y0) / dy;
        float x_intercept_times_dy = x0 * dy + (x1 - x0) * (y - y0);

        // text x_intercept > x
				
        // factor out the division; two cases depending on sign of dy
        if (cross_up)
        {
          ASSERT(dy > 0);
          if (x_intercept_times_dy > x * dy)
          {
            ray_crossings++;
          }
        }
        else
        {
          // dy is negative; reverse the inequality test
          ASSERT(dy < 0);
          if (x_intercept_times_dy < x * dy)
          {
            ray_crossings++;
          }
        }
      }
    }
    else
    {
      // Curve case.
      float cx = e.cx;
      float cy = e.cy;

      // Find whether & where the curve crosses y
      if ((y0 < y && y1 < y && cy < y)
          || (y0 > y && y1 > y && cy > y))
      {
        // All above or all below -- no possibility of crossing.
      }
      else if (x0 < x && x1 < x && cx < x)
      {
        // All to the left -- no possibility of crossing to the right.
      }
      else
      {
        // Find points where the curve crosses y.

        // Quadratic bezier is:
        //
        // p = (1-t)^2 * a0 + 2t(1-t) * c + t^2 * a1
        //
        // We need to solve for x at y.
					
        // Use the quadratic formula.

        // Numerical Recipes suggests this variation:
        // q = -0.5 [b +sgn(b) sqrt(b^2 - 4ac)]
        // x1 = q/a;  x2 = c/q;

        float A = y1 + y0 - 2 * cy;
        float B = 2 * (cy - y0);
        float C = y0 - y;

        float rad = B * B - 4 * A * C;
        if (rad < 0)
        {
          // No real solutions.
        }
        else
        {
          float q;
          float sqrt_rad = sqrtf(rad);
          if (B < 0) {
            q = -0.5f * (B - sqrt_rad);
          } else {
            q = -0.5f * (B + sqrt_rad);
          }

          // The old-school way.
          // float t0 = (-B + sqrt_rad) / (2 * A);
          // float t1 = (-B - sqrt_rad) / (2 * A);

          if (A != 0)
          {
            float t0 = q / A;
            if (t0 >= 0 && t0 < 1) {
              float x_at_t0 =
									x0 + 2 * (cx - x0) * t0 + (x1 + x0 - 2 * cx) * t0 * t0;
              if (x_at_t0 > x) {
                ray_crossings++;
              }
            }
          }

          if (q != 0)
          {
            float t1 = C / q;
            if (t1 >= 0 && t1 < 1) {
              float x_at_t1 =
									x0 + 2 * (cx - x0) * t1 + (x1 + x0 - 2 * cx) * t1 * t1;
              if (x_at_t1 > x) {
                ray_crossings++;
              }
            }
          }
        }
      }
    }

    x0 = x1;
    y0 = y1;
  }

  if (ray_crossings & 1)
  {
    // Odd number of ray crossings means the point
    // is inside the poly.
    return true;
  }
  return false;
}

// Push this path into the tesselator.
//void	Path::tesselate() const
//{
//  Tesselate::beginPath(
//			fill0 - 1,
//			fill1 - 1,
//			line - 1,
//			ax, ay);
//  for (int i = 0; i < edges.size(); i++)
//  {
//    edges[i].tesselateCurve();
//  }
//  Tesselate::endPath();
//}

// Push this path into the tesselator.
void	Path::tesselateNew() const
{
  TesselateNew::beginPath(
			fill0 - 1,
			fill1 - 1,
			line - 1,
			ax, ay);
  for (int i = 0; i < edges.size(); i++)
  {
    edges[i].tesselateCurveNew();
  }
  TesselateNew::endPath();
}

template<class T>
T	readLE(File* in);

template<>
float	readLE<float>(File* in)
{
  return in->readFloat32();
}

template<>
Sint16    readLE<Sint16>(File* in)
{
  return in->readUI16();
}

// Read the coordinate array data from the stream into *pt_array.
void	readCoordArray(File* in, std::vector<coord_component>* pt_array)
{
  int	n = in->readUI32();

  pt_array->resize(n);
  for (int i = 0; i < n; i ++)
  {
    (*pt_array)[i] = readLE<coord_component>(in);
  }
}

void	Mesh::setTriStrip(const Point pts[], int count)
{
  _triangleStrip.resize(count * 2);	// 2 coords per point
		
  // convert to ints.
  for (int i = 0; i < count; i++)
  {
    _triangleStrip[i * 2] = coord_component(pts[i].x);
    _triangleStrip[i * 2 + 1] = coord_component(pts[i].y);
  }

  //		m_triangle_strip.resize(count);
  //		memcpy(&m_triangle_strip[0], &pts[0], count * sizeof(point));
}

void Mesh::reserveTriangles(int expected_triangle_count)
{
  _triangleList.reserve(expected_triangle_count * 6);  // 6 coords per triangle
}


void Mesh::addTriangle(const coord_component pts[6])
{
  _triangleList.insert(_triangleList.end(), pts, pts+6);
}

//void	mesh::display(const base_fill_style& style, float ratio, render_handler::bitmap_blend_mode bm) const
//	{
//		// pass mesh to renderer.
//		if (m_triangle_strip.size() > 0)
//		{
//			style.apply(0, ratio, bm);
//			render::draw_mesh_strip(&m_triangle_strip[0], m_triangle_strip.size() >> 1);
//		}
//		if (m_triangle_list.size() > 0) {
//			style.apply(0, ratio, bm);
//			render::draw_triangle_list(&m_triangle_list[0], m_triangle_list.size() >> 1);
//		}
//	}

void	Mesh::outputCachedData(File* out)
    // Dump our data to *out.
{
  //write_coord_array(out, m_triangle_strip);
  //write_coord_array(out, m_triangle_list);
}

// Slurp our data from *out.
void	Mesh::inputCachedData(File* in)
{
  readCoordArray(in, &_triangleStrip);
  readCoordArray(in, &_triangleList);
}

LineStrip::LineStrip():_style(-1){}

// Construct the line strip (polyline) made up of the given sequence of points.
LineStrip::LineStrip(int style, const Point coords[], int coord_count):_style(style)
{
  ASSERT(style >= 0);
  ASSERT(coords != NULL);
  ASSERT(coord_count > 1);

  //		m_coords.resize(coord_count);
  //		memcpy(&m_coords[0], coords, coord_count * sizeof(coords[0]));
  _coords.resize(coord_count * 2);	// 2 coords per vert
		
  // convert to ints.
  for (int i = 0; i < coord_count; i++)
  {
    _coords[i * 2] = coord_component(coords[i].x);
    _coords[i * 2 + 1] = coord_component(coords[i].y);
  }
}


//void	line_strip::display(const base_line_style& style, float ratio) const
//    // Render this line strip in the given style.
//{
//  assert(m_coords.size() > 1);
//  assert((m_coords.size() & 1) == 0);
//
//  style.apply(ratio);
//  render::draw_line_strip(&m_coords[0], m_coords.size() >> 1);
//}


//void	LineStrip::output_cached_data(tu_file* out)
//    // Dump our data to *out.
//{
//  out->write_le32(m_style);
//  write_coord_array(out, m_coords);
//}

// Slurp our data from *out.	
void	LineStrip::inputCachedData(File* in)
{
  _style = in->readUI32();
  readCoordArray(in, &_coords);
}


//
// mesh_set
//


MeshSet::MeshSet():_errorTolerance(0)	// invalid -- don't use this constructor; it's only here for array (@@ fix array)
{
}

// Tesselate the shape's paths into a different mesh for each fill style.
MeshSet::MeshSet(const Tesselate::TesselatingShape* sh, float error_tolerance):_errorTolerance(error_tolerance)
{
  // For collecting trapezoids emitted by the old tesselator.
  //struct collect_traps : public tesselate::trapezoid_accepter
  //{
  //  mesh_set*	m;	// the mesh_set that receives trapezoids.
  //  bool m_new_layer;
  //
  //  // strips-in-progress.
  //  hash<int, tri_stripper*>	m_strips;
  //
  //  collect_traps(mesh_set* set) : m(set), m_new_layer(true) {}
  //  virtual ~collect_traps() {}
  //
  //  // Overrides from trapezoid_accepter
  //  virtual void	accept_trapezoid(int style, const tesselate::trapezoid& tr)
  //  {
  //    // Add trapezoid to appropriate stripper.
  //
  //    tri_stripper*	s = NULL;
  //    m_strips.get(style, &s);
  //    if (s == NULL)
  //    {
  //      s = new tri_stripper;
  //      m_strips.add(style, s);
  //    }
  //
  //    s->add_trapezoid(
	//				point(tr.m_lx0, tr.m_y0),
	//				point(tr.m_rx0, tr.m_y0),
	//				point(tr.m_lx1, tr.m_y1),
	//				point(tr.m_rx1, tr.m_y1));
  //  }
  //
  //  virtual void	accept_line_strip(int style, const point coords[], int coord_count)
  //      // Remember this line strip in our mesh set.
  //  {
  //    if (m_new_layer) {
  //      m->new_layer();
  //      m_new_layer = false;
  //    }
  //    m->add_line_strip(style, coords, coord_count);
  //  }
  //
  //  void	flush()
  //      // Push our strips into the mesh set.
  //  {
  //    if (m_new_layer) {
  //      m->new_layer();
  //      m_new_layer = false;
  //    }
  //    for (hash<int, tri_stripper*>::const_iterator it = m_strips.begin();
  //         it != m_strips.end();
  //         ++it)
  //    {
  //      // Push strip into m.
  //      tri_stripper*	s = it->second;
  //      s->flush(m, it->first);
	//				
  //      delete s;
  //    }
  //  }
  //
  //  void end_shape() {
  //    m_new_layer = true;
  //  }
  //};

  // For collecting triangles emitted by the new tesselator.
  class CollectTris : public TesselateNew::MeshAccepter
  {
  public:
    MeshSet*	ms;	// the mesh_set that receives triangles.
    Mesh* m;
    bool new_layer;

    CollectTris(MeshSet* set) : ms(set), m(NULL), new_layer(true) {
    }
    virtual ~CollectTris() {}

    // Overrides from mesh_accepter

    // Remember this line strip in our mesh set.
    virtual void	acceptLineStrip(int style, const Point coords[], int coord_count)
    {
      if (new_layer) {
        ms->newLayer();
        new_layer = false;
      }
      ms->addLineStrip(style, coords, coord_count);
    }

    virtual void beginTrilist(int style, int expected_triangle_count)
    {
      ASSERT(m == NULL);
      if (new_layer) {
        ms->newLayer();
        new_layer = false;
      }
      m = ms->getMutableMesh(style);
      m->reserveTriangles(expected_triangle_count);
    }

    // Accept one or more triangles to add to the
    // mesh for the specified style.
    virtual void acceptTrilistBatch(const Point trilist[], int point_count)
    {
      ASSERT(m != NULL);
				
      // Convert input from float coords to
      // coord_component and add them to the mesh.
      coord_component tri[6];
      for (int i = 0; i < point_count; i += 3) {
        tri[0] = static_cast<coord_component>(trilist[i].x);
        tri[1] = static_cast<coord_component>(trilist[i].y);
        tri[2] = static_cast<coord_component>(trilist[i + 1].x);
        tri[3] = static_cast<coord_component>(trilist[i + 1].y);
        tri[4] = static_cast<coord_component>(trilist[i + 2].x);
        tri[5] = static_cast<coord_component>(trilist[i + 2].y);
        m->addTriangle(tri);
      }
    }

    virtual void endTrilist()
    {
      m = NULL;
    }

    virtual void endShape()
    {
      new_layer = true;
    }
  };

//#ifndef USE_NEW_TESSELATOR
//  // Old tesselator.
//  collect_traps	accepter(this);
//  sh->tesselate(error_tolerance, &accepter);
//  accepter.flush();
//#else  // USE_NEW_TESSELATOR
  // New tesselator.
  CollectTris	accepter(this);
  sh->tesselateNew(error_tolerance, &accepter);
  //#endif // USE_NEW_TESSELATOR

  // triangles should be collected now into the meshes for each fill style.
}


MeshSet::Layer::~Layer() {
  for (int i = 0; i < _lineStrips.size(); i++) {
    delete _lineStrips[i];
  }
  for (int i = 0; i < _meshes.size(); i++) {
    delete _meshes[i];
  }
}

// Make room for a new layer.
void MeshSet::newLayer()
{
  _layers.resize(_layers.size() + 1);
}

//void	mesh_set::display(
//		const matrix& mat,
//		const cxform& cx,
//		const array<fill_style>& fills,
//		const array<line_style>& line_styles, render_handler::bitmap_blend_mode bm) const
//    // Throw our meshes at the renderer.
//{
//  assert(m_error_tolerance > 0);
//
//  // Setup transforms.
//  render::set_matrix(mat);
//  render::set_cxform(cx);
//
//  // Dump layers into renderer.
//  for (int j = 0; j < m_layers.size(); j++) {
//    const layer& l = m_layers[j];
//			
//    // Dump meshes into renderer, one mesh per style.
//    for (int i = 0; i < l.m_meshes.size(); i++) {
//      if (l.m_meshes[i]) {
//        l.m_meshes[i]->display(fills[i], 1.0f, bm);
//      }
//    }
//
//    // Dump line-strips into renderer.
//    {for (int i = 0; i < l.m_line_strips.size(); i++)
//			{
//				int	style = l.m_line_strips[i]->get_style();
//				l.m_line_strips[i]->display(line_styles[style], 1.0f);
//			}}
//  }
//}


void MeshSet::expandStylesToInclude(int style)
    // 
{
  ASSERT(style >= 0);
  ASSERT(style < 10000);	// sanity check

  Layer* l = &_layers.back();

  // Expand our mesh list if necessary.
  if (style >= l->_meshes.size()) {
    l->_meshes.resize(style + 1);
  }

  if (l->_meshes[style] == NULL) {
    l->_meshes[style] = new Mesh;
  }
}

// Set mesh associated with the given fill style to the
// specified triangle strip.
void	MeshSet::setTriStrip(int style, const Point pts[], int count)
{
  expandStylesToInclude(style);
  _layers.back()._meshes[style]->setTriStrip(pts, count);
}

Mesh* MeshSet::getMutableMesh(int style)
{
  expandStylesToInclude(style);
  return _layers.back()._meshes[style];
}

// Add the specified line strip to our list of things to render.
void	MeshSet::addLineStrip(int style, const Point coords[], int coord_count)
{
  ASSERT(style >= 0);
  ASSERT(style < 1000);	// sanity check
  ASSERT(coords != NULL);
  ASSERT(coord_count > 1);

  _layers.back()._lineStrips.push_back(new LineStrip(style, coords, coord_count));
}


//void	mesh_set::output_cached_data(tu_file* out)
//    // Dump our data to the output stream.
//{
//  out->write_float32(m_error_tolerance);
//
//  int layer_n = m_layers.size();
//  out->write_le32(layer_n);
//
//  for (int j = 0; j < layer_n; j++) {
//    const layer& l = m_layers[j];
//			
//    int	mesh_n = l.m_meshes.size();
//    out->write_le32(mesh_n);
//    for (int i = 0; i < mesh_n; i++)
//    {
//      if (l.m_meshes[i]) {
//        out->write_byte(1);
//        l.m_meshes[i]->output_cached_data(out);
//      } else {
//        out->write_byte(0);
//      }
//    }
//
//    int	lines_n = l.m_line_strips.size();
//    out->write_le32(lines_n);
//    {for (int i = 0; i < lines_n; i++)
//			{
//				l.m_line_strips[i]->output_cached_data(out);
//			}}
//  }
//}

// Grab our data from the input stream.
void	MeshSet::inputCachedData(File* in)
{
  _errorTolerance = in->readFloat32();

  int layer_n = in->readUI32();
  _layers.resize(layer_n);
  for (int j = 0; j < layer_n; j++) {
    Layer* l = &_layers[j];

    int	mesh_n = in->readUI32();
    l->_meshes.resize(mesh_n);
    for (int i = 0; i < mesh_n; i++)
    {
      bool non_null = in->readByte() > 0;
      if (non_null) {
        l->_meshes[i] = new Mesh;
        l->_meshes[i]->inputCachedData(in);
      }
    }

    int	lines_n = in->readUI32();
    l->_lineStrips.resize(lines_n);
    for (int i = 0; i < lines_n; i++)
    {
      l->_lineStrips[i] = new LineStrip;
      l->_lineStrips[i]->inputCachedData(in);
    }
  }
}

// Read fill styles, and push them onto the given style array.
static void	readFillStyles(std::vector<FillStyle>* styles, Stream* in, int tag_type, MovieDefinitionSub* m)
{
  ASSERT(styles);

  // Get the count.
  int	fill_style_count = in->readU8();
  if (tag_type > 2)//Tag::DefineShape
  {
    if (fill_style_count == 0xFF)
    {
      fill_style_count = in->readU16();
    }
  }

  std::printf("  read_fill_styles: count = %d\n", fill_style_count);

  // Read the styles.
  for (int i = 0; i < fill_style_count; i++)
  {
    (*styles).resize((*styles).size() + 1);
    (*styles)[(*styles).size() - 1].read(in, tag_type, m);
  }
}

// Read line styles and push them onto the back of the given array.
static void	readLineStyles(std::vector<LineStyle>* styles, Stream* in, int tag_type, MovieDefinitionSub* m)
{
  // Get the count.
  int	line_style_count = in->readU8();

  std::printf("  read_line_styles: count = %d\n", line_style_count);

  // @@ does the 0xFF flag apply to all tag types?
  // if (tag_type > 2)
  // {
  if (line_style_count == 0xFF)
  {
    line_style_count = in->readU16();
    std::printf("  read_line_styles: count2 = %d\n", line_style_count);
  }
  // }

  // Read the styles.
  for (int i = 0; i < line_style_count; i++)
  {
    (*styles).resize((*styles).size() + 1);
    (*styles)[(*styles).size() - 1].read(in, tag_type, m);
  }
}

void ShapeCharacterDef::read(Stream* in, int tag_type, bool with_style, MovieDefinitionSub* m)
{
  if (with_style)
  {
    _bound.read(in);
	
    // DefineShape4, Flash 8
    if (tag_type == 83)
    {
      _edgeBounds.read(in);
      Uint8 b = in->readUI8();
      ASSERT( (b & 0xFC) == 0 );
      _usesNonscalingStrokes = b & 0x02 ? true : false;
      _usesScalingStrokes = b & 0x01 ? true : false;
    }

    readFillStyles(&_fillStyles, in, tag_type, m);
    readLineStyles(&_lineStyles, in, tag_type, m);
  }

  //
  // SHAPE
  //
  int	num_fill_bits = in->readUint(4);
  int	num_line_bits = in->readUint(4);

  std::printf("  shape_character read: nfillbits = %d, nlinebits = %d\n", num_fill_bits, num_line_bits);

  // These are state variables that keep the
  // current position & style of the shape
  // outline, and vary as we read the edge data.
  //
  // At the moment we just store each edge with
  // the full necessary info to render it, which
  // is simple but not optimally efficient.
  int	fill_base = 0;
  int	line_base = 0;
  float	x = 0, y = 0;
  Path	current_path;

#define SHAPE_LOG 0
  // SHAPERECORDS
  for (;;) {
    int	type_flag = in->readUint(1);
    if (type_flag == 0)
    {
      // Parse the record.
      int	flags = in->readUint(5);
      if (flags == 0) {
        // End of shape records.

        // Store the current path if any.
        if (! current_path.isEmpty())
        {
          _paths.push_back(current_path);
          current_path.edges.resize(0);
        }

        break;
      }
      if (flags & 0x01)
      {
        // move_to = 1;

        // Store the current path if any, and prepare a fresh one.
        if (! current_path.isEmpty())
        {
          _paths.push_back(current_path);
          current_path.edges.resize(0);
        }

        int	num_move_bits = in->readUint(5);
        int	move_x = in->readSint(num_move_bits);
        int	move_y = in->readSint(num_move_bits);

        x = (float) move_x;
        y = (float) move_y;

        // Set the beginning of the path.
        current_path.ax = x;
        current_path.ay = y;

        if (SHAPE_LOG) std::printf("  shape_character read: moveto %4g %4g\n", x, y);
      }
      if ((flags & 0x02)
					&& num_fill_bits > 0)
      {
        // fill_style_0_change = 1;
        if (! current_path.isEmpty())
        {
          _paths.push_back(current_path);
          current_path.edges.resize(0);
          current_path.ax = x;
          current_path.ay = y;
        }
        int	style = in->readUint(num_fill_bits);
        if (style > 0)
        {
          style += fill_base;
        }
        current_path.fill0 = style;
        if (SHAPE_LOG) std::printf("  shape_character read: fill0 = %d\n", current_path.fill0);
      }
      if ((flags & 0x04)
					&& num_fill_bits > 0)
      {
        // fill_style_1_change = 1;
        if (! current_path.isEmpty())
        {
          _paths.push_back(current_path);
          current_path.edges.resize(0);
          current_path.ax = x;
          current_path.ay = y;
        }
        int	style = in->readUint(num_fill_bits);
        if (style > 0)
        {
          style += fill_base;
        }
        current_path.fill1 = style;
        if (SHAPE_LOG) std::printf("  shape_character read: fill1 = %d\n", current_path.fill1);
      }
      if ((flags & 0x08)
					&& num_line_bits > 0)
      {
        // line_style_change = 1;
        if (! current_path.isEmpty())
        {
          _paths.push_back(current_path);
          current_path.edges.resize(0);
          current_path.ax = x;
          current_path.ay = y;
        }
        int	style = in->readUint(num_line_bits);
        if (style > 0)
        {
          style += line_base;
        }
        current_path.line = style;
        if (SHAPE_LOG) std::printf("  shape_character_read: line = %d\n", current_path.line);
      }
      if (flags & 0x10) {
        ASSERT(tag_type >= 22);

        std::printf("  shape_character read: more fill styles\n");

        // Store the current path if any.
        if (! current_path.isEmpty())
        {
          _paths.push_back(current_path);
          current_path.edges.resize(0);

          // Clear styles.
          current_path.fill0 = -1;
          current_path.fill1 = -1;
          current_path.line = -1;
        }
        // Tack on an empty path signalling a new shape.
        // @@ need better understanding of whether this is correct??!?!!
        // @@ i.e., we should just start a whole new shape here, right?
        _paths.push_back(Path());
        _paths.back().newShape = true;

        fill_base = _fillStyles.size();
        line_base = _lineStyles.size();
        readFillStyles(&_fillStyles, in, tag_type, m);
        readLineStyles(&_lineStyles, in, tag_type, m);
        num_fill_bits = in->readUint(4);
        num_line_bits = in->readUint(4);
      }
    }
    else
    {
      // EDGERECORD
      int	edge_flag = in->readUint(1);
      if (edge_flag == 0)
      {
        // curved edge
        int num_bits = 2 + in->readUint(4);
        float	cx = x + in->readSint(num_bits);
        float	cy = y + in->readSint(num_bits);
        float	ax = cx + in->readSint(num_bits);
        float	ay = cy + in->readSint(num_bits);

        if (SHAPE_LOG) std::printf("  shape_character read: curved edge   = %4g %4g - %4g %4g - %4g %4g\n", x, y, cx, cy, ax, ay);

        current_path.edges.push_back(Edge(cx, cy, ax, ay));

        x = ax;
        y = ay;
      }
      else
      {
        // straight edge
        int	num_bits = 2 + in->readUint(4);
        int	line_flag = in->readUint(1);
        float	dx = 0, dy = 0;
        if (line_flag)
        {
          // General line.
          dx = (float) in->readSint(num_bits);
          dy = (float) in->readSint(num_bits);
        }
        else
        {
          int	vert_flag = in->readUint(1);
          if (vert_flag == 0) {
            // Horizontal line.
            dx = (float) in->readSint(num_bits);
          } else {
            // Vertical line.
            dy = (float) in->readSint(num_bits);
          }
        }

        if (SHAPE_LOG) std::printf("  shape_character_read: straight edge = %4g %4g - %4g %4g\n", x, y, x + dx, y + dy);

        current_path.edges.push_back(Edge(x + dx, y + dy, x + dx, y + dy));

        x += dx;
        y += dy;
      }
    }
  }
}

// Push our shape data through the tesselator.
void ShapeCharacterDef::tesselateNew(float error_tolerance, TesselateNew::MeshAccepter *accepter) const
{
  TesselateNew::beginShape(accepter, error_tolerance);
  for (int i = 0; i < _paths.size(); i++)
  {
    if (_paths[i].newShape == true)
    {
      // Hm; should handle separate sub-shapes in a less lame way.
      TesselateNew::endShape();
      TesselateNew::beginShape(accepter, error_tolerance);
    }
    else
    {
      _paths[i].tesselateNew();
    }
  }
  TesselateNew::endShape();
}
    
