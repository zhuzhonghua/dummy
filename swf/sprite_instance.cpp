#include "stdafx.h"

#include "sprite_instance.h"
#include "movie.h"
#include "execute_tag.h"
#include "movie_definition_sub.h"

SpriteInstance::SpriteInstance(MovieDefinitionSub* def,Movie* m):
	_currentFrame(0),
	_movie(m),
	_def(def)
{
}

void SpriteInstance::advance(float deltaTime)
{
	int prevFrame = _currentFrame;

	_currentFrame++;
	if (_currentFrame >= _def->getFrameCount()) {
    _currentFrame = 0;
	}

	if(prevFrame != _currentFrame){
		executeFrameTags(_currentFrame);
	}

	_displayList.advance(deltaTime);
}

void SpriteInstance::display()
{
	_displayList.display();
}

void SpriteInstance::executeFrameTags(int frame, bool stateOnly)
{
	ASSERT(frame >= 0);
	ASSERT(frame < _def->getFrameCount());

	const std::vector<ExecuteTag*>& playList =_def->getPlaylist(frame);
	for (int i = 0; i < playList.size(); ++i) {
    ExecuteTag* e = playList[i];
		e->execute(this);
	}
}

void SpriteInstance::setBackgroundColor(const RGBA& color)
{
	_movie->setBackgroundColor(color);
}

