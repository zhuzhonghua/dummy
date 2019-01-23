#pragma once

#include "character_def.h"
#include "styles.h"

namespace TesselateNew {
class MeshAccepter;
};

namespace Tesselate {
class TrapezoidAccepter;
class TesselatingShape {
public:
  virtual ~TesselatingShape() {}
  //virtual void tesselate(float error_tolerance, TrapezoidAccepter *accepter) const = 0;
  virtual void tesselateNew(float error_tolerance, TesselateNew::MeshAccepter *accepter) const = 0;
};
};

class Edge
// Together with the previous anchor, defines a quadratic
// curve segment.
{
public:
  Edge();
  Edge(float cx, float cy, float ax, float ay);
  void	tesselateCurve() const;
  void	tesselateCurveNew() const;
  bool	isStraight() const;
		
	//private:
  // *quadratic* bezier: point = p0 * t^2 + p1 * 2t(1-t) + p2 * (1-t)^2
  float	cx, cy;		// "control" point
  float	ax, ay;		// "anchor" point
};

class Path
// A subset of a shape -- a series of edges sharing a single set
// of styles.
{
public:
  Path();
  Path(float ax, float ay, int fill0, int fill1, int line);

  void	reset(float ax, float ay, int fill0, int fill1, int line);
  bool	isEmpty() const;

  bool	pointTest(float x, float y);

  // Push the path into the tesselator.
  void	tesselate() const;
  void	tesselateNew() const;

	//private:
  int	fill0, fill1, line;
  float	ax, ay;	// starting point
  std::vector<Edge>	edges;
  bool	newShape;
};

class Mesh
// For holding a pre-tesselated shape.
{
public:
  Mesh(){}

  void setTriStrip(const Point pts[], int count); // TODO remove
  void reserveTriangles(int expected_triangle_count);
  void addTriangle(const coord_component pts[6]);

  //void	display(const base_fill_style& style, float ratio, render_handler::bitmap_blend_mode bm) const;

  void	outputCachedData(File* out);
  void	inputCachedData(File* in);
private:
  std::vector<coord_component> _triangleStrip;// TODO remove
  std::vector<coord_component> _triangleList;
};

class LineStrip
// For holding a line-strip (i.e. polyline).
{
public:
  LineStrip();
  LineStrip(int style, const Point coords[], int coord_count);

  //void	display(const base_line_style& style, float ratio) const;

  int	getStyle() const { return _style; }
  //void	output_cached_data(File* out);
  void	inputCachedData(File* in);
private:
  int	_style;
  std::vector<coord_component>	_coords;
};

class MeshSet
// A whole shape, divided into layers and tesselated to a
// certain error tolerance.
{
public:
  MeshSet();
  MeshSet(const Tesselate::TesselatingShape* sh,
           float error_tolerance);
  ~MeshSet(){}

  float	getErrorTolerance() const { return _errorTolerance; }

  //void display( const matrix& m, const cxform& cx, const array<fill_style>& fills,
  //              const array<line_style>& line_styles, render_handler::bitmap_blend_mode bm) const;

  void 	newLayer();
  void	setTriStrip(int style, const Point pts[], int count);
  void	addLineStrip(int style, const Point coords[], int coord_count);

  Mesh* getMutableMesh(int style);
		
  //void	output_cached_data(tu_file* out);
  void	inputCachedData(File* in);

private:
  void expandStylesToInclude(int style);
		
  float	_errorTolerance;
  struct Layer {
    std::vector<Mesh*> _meshes;  // one mesh per style.
    std::vector<LineStrip*> _lineStrips;

    ~Layer();  // delete m_meshes & m_line_strips
  };
  std::vector<Layer> _layers;
};

class MovieDefinitionSub;
class ShapeCharacterDef:public CharacterDef, public Tesselate::TesselatingShape{
public:
  void	read(Stream* in, int tag_type, bool with_style, MovieDefinitionSub* m);
  
  virtual void tesselateNew(float error_tolerance, TesselateNew::MeshAccepter *accepter) const;
protected:
  // derived morph classes changes these
  std::vector<FillStyle>	_fillStyles;
  std::vector<LineStyle>	_lineStyles;
  std::vector<Path>				_paths;
private:
  //void	sort_and_clean_meshes() const;
		
  RECT	_bound;

  // for DefineShape4
  RECT	_edgeBounds;
  bool	_usesNonscalingStrokes;
  bool	_usesScalingStrokes;
};
