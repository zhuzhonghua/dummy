#include "stdafx.h"
#include "movie_def_impl.h"
#include "swf_player.h"
#include "render/render.h"

SWFPlayer::SWFPlayer(const std::string& f)
  :_file(f.c_str())
{
  _mdef = new MovieDefImpl(this);
}

SWFPlayer::~SWFPlayer()
{
  delete _mdef;
}

float SWFPlayer::getFrameRate()
{
  return _mdef->getFrameRate();
}

int SWFPlayer::getWidth()
{
  return _mdef->getWidthPixels();
}

int SWFPlayer::getHeight()
{
  return _mdef->getHeightPixels();
}

void SWFPlayer::readHead()
{
  _mdef->readHead(&_file);
}

void SWFPlayer::readTags()
{
  _mdef->readTags();
}

void SWFPlayer::advance()
{
}

void SWFPlayer::display()
{
  Render::setBGColor(_bgColor);
}

void SWFPlayer::setBGColor(RGBA color)
{
  _bgColor = color;
}
