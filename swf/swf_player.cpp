#include "stdafx.h"
#include "sprite_instance.h"
#include "swf_player.h"
#include "render/render.h"

SWFPlayer::SWFPlayer(const std::string& f)
  :_file(f.c_str())
{
  _sprite = new SpriteInstance(this, &_file);
}

SWFPlayer::~SWFPlayer()
{
  delete _sprite;
}

float SWFPlayer::getFrameRate()
{
  return _sprite->getFrameRate();
}

int SWFPlayer::getWidth()
{
  return _sprite->getWidth();
}

int SWFPlayer::getHeight()
{
  return _sprite->getHeight();
}

void SWFPlayer::readHead()
{
  _sprite->readHead();
}

void SWFPlayer::readTags()
{
  _sprite->readTags();
}

void SWFPlayer::advance()
{
	_sprite->advance();
}

void SWFPlayer::display()
{
  Render::setBGColor(_sprite->getBGColor());
	//TODO: sprite instance display
	_sprite->display();
}
