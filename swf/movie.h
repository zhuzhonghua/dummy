#pragma once

#include "basic_types.h"

class MovieDefImpl;
class Character;

class Movie{
public:
	Movie(MovieDefImpl* def);

	int 	getWidth();
	int 	getHeight();

	void	display();
	void	setMovie(Character* movie){ _movie = movie; }
	void 	advance(float deltaTime);
	Character* getMovie() { return _movie; }

	void	setBackgroundColor(const RGBA& color){ _bgColor = color; }
	RGBA	getBGColor() { return _bgColor; }
private:
	MovieDefImpl* _def;
	Character*		_movie;

	RGBA					_bgColor;
};
