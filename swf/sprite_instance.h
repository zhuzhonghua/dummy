#pragma once

#include "character.h"
#include "display_list.h"

class MovieDefinitionSub;
class Movie;

class SpriteInstance:public Character{
public:
	// Unique id of a gameswf resource
	enum { _classId = AS_SPRITE };
	virtual bool is(int classId) const
	{
		if (_classId == classId) return true;
		else return Character::is(classId);
	}
	
	SpriteInstance(MovieDefinitionSub* def, Movie* m);

	virtual void	advance(float deltaTime);
	virtual void	display();
	virtual void	executeFrameTags(int frame, bool stateOnly = false);
	virtual void	setBackgroundColor(const RGBA& color);

	void	setMovie(Movie* m) { _movie = m; }
private:
	MovieDefinitionSub* _def;
	DisplayList					_displayList;
	int									_currentFrame;

private:
	Movie* 							_movie;
};

