#pragma once

#include "character_def.h"
#include "styles.h"

class Edge
// Together with the previous anchor, defines a quadratic
// curve segment.
{
public:
  Edge();
  Edge(float cx, float cy, float ax, float ay);
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

	//private:
  int	fill0, fill1, line;
  float	ax, ay;	// starting point
  std::vector<Edge>	edges;
  bool	newShape;
};

class MovieDefinitionSub;
class ShapeCharacterDef:public CharacterDef{
public:
  void	read(Stream* in, int tag_type, bool with_style, MovieDefinitionSub* m);

protected:
  // derived morph classes changes these
  std::vector<FillStyle>	_fillStyles;
  std::vector<LineStyle>	_lineStyles;
  std::vector<Path>				_paths;
private:
		
  RECT	_bound;

  // for DefineShape4
  RECT	_edgeBounds;
  bool	_usesNonscalingStrokes;
  bool	_usesScalingStrokes;
};
