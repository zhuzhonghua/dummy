#pragma once

#include <vector>
#include "character.h"

// A struct to serve as an entry in the display list.
struct DisplayObjectInfo
{
  Character*	character;	// state is held in here

  DisplayObjectInfo()
  {
  }

  DisplayObjectInfo(const DisplayObjectInfo& di)
  {
    *this = di;
  }

  ~DisplayObjectInfo()
  {
  }

  void	operator=(const DisplayObjectInfo& di)
  {
    character = di.character;
  }

  void	setCharacter(Character* ch)
  {
    character = ch;
  }

  static int compare(const void* _a, const void* _b); // For qsort().
};

class DisplayList{
public:
  // advance referenced characters.
  void	advance(float deltaTime);

  // display the referenced characters.
  void	display();
  void	display(const DisplayInfo& di);

	void	addDisplayObject(Character* ch, int depth, const Matrix& mat);

protected:
	int findIndexByDepth(int depth);
private:
  void remove(int index);
  std::vector<DisplayObjectInfo> _displayObjectArray;
};
