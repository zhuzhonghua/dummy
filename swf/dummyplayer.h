#pragma once

#include "player.h"

class Movie;

class DummyPlayer:public Player{
public:
	bool init(const std::string& file);

	virtual void update();
	virtual void draw();
private:
	Movie*	_movie;
};
