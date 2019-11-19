#include "stdafx.h"

#include "shape_character_def.h"
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

  INFO("  read_fill_styles: count = %d", fill_style_count);

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

  INFO("  read_line_styles: count = %d", line_style_count);

  // @@ does the 0xFF flag apply to all tag types?
  // if (tag_type > 2)
  // {
  if (line_style_count == 0xFF)
  {
    line_style_count = in->readU16();
    INFO("  read_line_styles: count2 = %d", line_style_count);
  }
  // }

  // Read the styles.
  for (int i = 0; i < line_style_count; i++)
  {
    (*styles).resize((*styles).size() + 1);
    (*styles)[(*styles).size() - 1].read(in, tag_type, m);
  }
}

ShapeCharacterDef::ShapeCharacterDef()
{	
}

ShapeCharacterDef::~ShapeCharacterDef()
{
}

void ShapeCharacterDef::read(Stream* in, int tag_type, bool with_style, MovieDefinitionSub* m)
{
	INFO("  with_style %d", with_style);
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

  INFO("  shape_character read: nfillbits = %d, nlinebits = %d", num_fill_bits, num_line_bits);

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

  // SHAPERECORDS
  for (;;) {
    int	type_flag = in->readUint(1);
		INFO("  type_flag=%d", type_flag);
    if (type_flag == 0)
    {
      // Parse the record.
      int	flags = in->readUint(5);
			INFO("  flags=%d", flags);
			
      if (flags == 0)
			{
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
      }
			
      if ((flags & 0x02) && num_fill_bits > 0)
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
      }
			
      if ((flags & 0x04) && num_fill_bits > 0)
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
      }
			
      if ((flags & 0x08) && num_line_bits > 0)
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
			INFO("  edge_flag=%d", edge_flag);
      if (edge_flag == 0)
      {
        // curved edge
        int num_bits = 2 + in->readUint(4);
        float	cx = x + in->readSint(num_bits);
        float	cy = y + in->readSint(num_bits);
        float	ax = cx + in->readSint(num_bits);
        float	ay = cy + in->readSint(num_bits);

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

        current_path.edges.push_back(Edge(x + dx, y + dy, x + dx, y + dy));

        x += dx;
        y += dy;
      }
    }
  }
}

void ShapeCharacterDef::display(Character* ch)
{
	// display fillstyle
	for (int i=0; i<_fillStyles.size(); i++)
	{
		_fillStyles[i].display(ch);
	}
}

void ShapeCharacterDef::getBound(RECT* bound)
{
	*bound = _bound;
}
