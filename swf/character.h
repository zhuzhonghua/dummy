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

class CharacterDef;

class Character:public ASObject{
public:
  // Unique id of a gameswf resource
  enum { _classId = AS_CHARACTER };
  virtual bool is(int classId) const
  {
    if (_classId == classId) return true;
    else return ASObject::is(classId);
  }

	
	Character(Character* parent, int id);
  virtual ~Character() {}
  virtual void	setBackgroundColor(const RGBA& color) {}

	virtual CharacterDef* getCharacterDef() { ASSERT(0); return NULL; }
	
  // Set to 0 if you don't want the movie to render its
  // background at all.  1 == full opacity.
  virtual void	setBackgroundAlpha(float alpha) {}

  virtual float	getBackgroundAlpha() const { return 1.0f; }
  virtual void	advance(float deltaTime) {}	// for buttons and sprites
  virtual void	display() {}
  virtual void	executeFrameTags(int frame, bool stateOnly = false) {}
  //virtual float	getHeight();
  //virtual float	getWidth();

	virtual void	addDisplayObject(int characterId, int depth, const Matrix& mat) {}
 
	void setDepth(int depth) { _depth = depth; }
	int getDepth() { return _depth; }

	void setMatrix(const Matrix& mat) { _mat = mat; }
	Matrix getMatrix() const { return _mat; }

	Matrix getWorldMatrix() const;

	virtual void getBound(RECT* bound);
protected:
	int _depth;
	int _id;
	Character* _parent;

	Matrix _mat;
};


class GenericCharacter : public Character{
public:
	GenericCharacter(CharacterDef* def, Character* parent, int id);
	virtual void display();

	virtual CharacterDef* getCharacterDef() { return _def; }
protected:
	CharacterDef* _def;
};
