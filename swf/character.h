#pragma once

#include "basic_types.h"

// Information about how to display a character.
struct DisplayInfo
{
  int	depth;
  CXForm	colorTransform;
  Matrix	matrix;
  float		ratio;
  UInt16	clipDepth;

  DisplayInfo() :
    depth(0),
    ratio(0.0f),
    clipDepth(0)
  {
  }

  void	concatenate(const DisplayInfo& di)
  // Concatenate the transforms from di into our
  // transforms.
  {
    depth = di.depth;
    colorTransform.concatenate(di.colorTransform);
    matrix.concatenate(di.matrix);
    ratio = di.ratio;
    clipDepth = di.clipDepth;
  }
};

class Character:public ASObject{
public:
  // Unique id of a gameswf resource
  enum { _classId = AS_CHARACTER };
  virtual bool is(int classId) const
  {
    if (_classId == classId) return true;
    else return ASObject::is(classId);
  }

  virtual ~Character() {}
  virtual void	setBackgroundColor(const RGBA& color) {}

  // Set to 0 if you don't want the movie to render its
  // background at all.  1 == full opacity.
  virtual void	setBackgroundAlpha(float alpha) {}

  virtual float	getBackgroundAlpha() const { return 1.0f; }
  virtual void	advance(float deltaTime) {}	// for buttons and sprites
  virtual void	display() {}
  virtual void	executeFrameTags(int frame, bool stateOnly = false) {}
  //virtual float	getHeight();
  //virtual float	getWidth();
};
