#pragma once

#include "def.h"

class Character;

class CharacterDef:public ASObjectInterface{
public:
	// Unique id of a gameswf resource
	enum { _classId = AS_CHARACTER_DEF };
	virtual bool is(int classId) const
	{
		return _classId == classId;
	}

	virtual void	display(Character* ch) {}
	//TODO: no need id, id should be same with def
	Character* getCharacterInst(Character* ch, int id);

protected:
	virtual Character* createCharacterInst(Character* parent, int id);

protected:
	Character* _chInst;
};

class BitmapCharacterDef:public CharacterDef{
public:
  virtual BitmapInfo*	getBitmapInfo() = 0;
};

class MovieDefinition;
class Character;

class BitmapCharacter:public BitmapCharacterDef{
public:
  BitmapCharacter(MovieDefinition* rdef, BitmapInfo* bi);

  // Return true if the specified point is on the interior of our shape.
  // Incoming coords are local coords.
  bool	pointTestLocal(float x, float y);

  virtual void getBound(RECT* bound);
  virtual void display(Character* ch);
  virtual BitmapInfo*	getBitmapInfo() { return _bitmapInfo; }

private:
  BitmapInfo* _bitmapInfo;
};
