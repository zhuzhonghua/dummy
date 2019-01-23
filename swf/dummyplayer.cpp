#include "stdafx.h"

#include "dummyplayer.h"
#include "movie.h"
#include "sprite_instance.h"

#include "render/image.h"
#include "noosascript.h"

Image* img;

bool DummyPlayer::init(const std::string& file)
{
	Movie* m = this->createMovie(file);
	Player::width = m->getWidth();
	Player::height = m->getHeight();
	
	this->_movie = m;
	
  bool flag = Player::init();

	// set bgcolor
	RGBA bgColor = _movie->getBGColor();
	glClearColor(bgColor._r, bgColor._g, bgColor._b, bgColor._a);
	
  //NoosaScript::get();
  //img = new Image("/Users/zhonghua/demo/swf/dummyplayer/test.png");
  
	return flag;
}

void DummyPlayer::update()
{
	Player::update();
	this->_movie->advance(Player::elapsed);
}

void DummyPlayer::draw()
{
	Player::draw();
	this->_movie->display();
  //img->draw();
}
