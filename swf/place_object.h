#pragma once

#include "execute_tag.h"
#include "swf_impl.h"

class Stream;
class Character;

class PlaceObject: public ExecuteTag
{
public:
  enum PlaceType {
    PLACE,
    MOVE,
    REPLACE,
  };
  
public:
	PlaceObject();
  ~PlaceObject();
  void		read(Stream* in, int tagType);
public:
  int	tagType;
  std::string	characterName;
  float	ratio;
  CXForm	colorTransform;
  Matrix	matrix;
  bool	hasMatrix;
  bool	hasCXForm;
  int 	depth;
  Uint16	characterId;
  Uint16	clipDepth;
  Uint8   blendMode;
  array<SWFEvent*> eventHandlers;
  
  PlaceType placeType;
};
