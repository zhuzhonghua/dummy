#pragma once

#include "execute_tag.h"

class Stream;
class Character;

class SetBackgroundColor: public ExecuteTag
{
public:
  void read(Stream* in);
  RGBA getColor() { return _color; }
private:
  RGBA		_color;
};
