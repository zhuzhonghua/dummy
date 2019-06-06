#include "stdafx.h"

#include "sprite_instance.h"
#include "movie_def_impl.h"

SpriteInstance::SpriteInstance(SWFPlayer* player, File* file)
	:Character(NULL, -1)
{
	_player = player;
	_file = file;
	
	_mdef = new MovieDefImpl(this);
}

SpriteInstance::~SpriteInstance()
{
	delete _mdef;
}

float SpriteInstance::getFrameRate()
{
  return _mdef->getFrameRate();
}

int SpriteInstance::getWidth()
{
  return _mdef->getWidthPixels();
}

int SpriteInstance::getHeight()
{
  return _mdef->getHeightPixels();
}

void SpriteInstance::readHead()
{
  _mdef->readHead(_file);
}

void SpriteInstance::readTags()
{
  _mdef->readTags();
}

void SpriteInstance::setBGColor(RGBA color)
{
	_bgColor = color;
}

void SpriteInstance::addDisplayObject(int characterId, int depth)
{
	CharacterDef* chDef = _mdef->getCharacter(characterId);
	if(chDef == NULL)
	{
		return;
	}

	Character* ch = chDef->getCharacterInst(this, characterId);
	_playList.addDisplayObject(ch, depth);
}
