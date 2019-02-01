#pragma once

#include "character_def.h"

//class Movie;

class MovieDefinition:public CharacterDef{
public:
  virtual float	getWidthPixels() const = 0;
  virtual float	getHeightPixels() const = 0;
  virtual int	getFrameCount() = 0;
  virtual float	getFrameRate() const = 0;

  //virtual Movie*	createInstance() = 0;
};
