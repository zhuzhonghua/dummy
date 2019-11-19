#include "stdafx.h"

#include "sprite_instance.h"
#include "movie_def_impl.h"
#include "execute_tag.h"

SpriteInstance::SpriteInstance(SWFPlayer* player, File* file)
	:Character(NULL, -1),
	 _currentFrame(0)
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

void SpriteInstance::addDisplayObject(int characterId, int depth, const Matrix& mat)
{
	CharacterDef* chDef = _mdef->getCharacter(characterId);
	if(chDef == NULL)
	{
		return;
	}

	Character* ch = chDef->getCharacterInst(this, characterId);
	_playList.addDisplayObject(ch, depth, mat);
}

void SpriteInstance::executeFrameTags(int frame, bool stateOnly)
{
	const std::vector<ExecuteTag*>&	playList = _mdef->getPlaylist(frame);
	for(int i=0; i<playList.size(); i++)
	{
		ExecuteTag* eTag = playList[i];
		eTag->execute(this);
	}
}

int SpriteInstance::getFrameCount()
{
	return _mdef->getFrameCount();
}

void SpriteInstance::advance()
{
	if(_currentFrame < getFrameCount())
	{
		executeFrameTags(_currentFrame);
		
		_currentFrame++;		
	}
}

void SpriteInstance::display()
{
	_playList.display();
}
