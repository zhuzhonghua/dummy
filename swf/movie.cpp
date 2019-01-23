#include "stdafx.h"
#include "movie.h"
#include "character.h"
#include "movie_def_impl.h"

Movie::Movie(MovieDefImpl* def)
{
	this->_def = def;
}

int Movie::getWidth()
{
	return this->_def->getWidthPixels();
}

int Movie::getHeight()
{
	return this->_def->getHeightPixels();
}

void Movie::advance(float deltaTime)
{
	this->_movie->advance(deltaTime);
}

void Movie::display()
{
	glClearColor(_bgColor._r, _bgColor._g, _bgColor._b, _bgColor._a);
	
	this->_movie->display();
}
