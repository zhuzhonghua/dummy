#pragma once

#include "execute_tag.h"

class Stream;
class Character;

class SetBackgroundColor: public ExecuteTag
{
public:
  void		read(Stream* in);
  // void		execute(Character* m);
  // void		executeState(Character* m);

private:
  RGBA		_color;
};
