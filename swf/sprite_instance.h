#pragma once

#include "character.h"
#include "display_list.h"

class MovieDefImpl;
class SWFPlayer;

class SpriteInstance : public Character{
public:
	enum { _classId = AS_SPRITE };
  virtual bool is(int classId) const
  {
    if (_classId == classId) return true;
    else return Character::is(classId);
  }

	SpriteInstance(SWFPlayer* player, File* file);
	~SpriteInstance();

	void readHead();
  void readTags();

  int getWidth();
  int getHeight();
  float getFrameRate();

  void setBGColor(RGBA color);
	RGBA getBGColor() { return _bgColor; }

	virtual void	addDisplayObject(int characterId, int depth);
protected:
	MovieDefImpl* _mdef;

	SWFPlayer* _player;
	File* _file;

	RGBA _bgColor;
  
  DisplayList _playList;
};
