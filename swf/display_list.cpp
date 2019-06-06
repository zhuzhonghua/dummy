#include "stdafx.h"

#include "display_list.h"
#include "character.h"

void DisplayList::display()
{
  for (int i = 0; i < _displayObjectArray.size(); ++i) {
    DisplayObjectInfo& doi = _displayObjectArray[i];

    Character* ch = doi.character;
    ch->display();
  }
}

void DisplayList::advance(float deltaTime)
{
  std::vector<DisplayObjectInfo> tmpList =  _displayObjectArray;
  for (int i = 0; i < tmpList.size(); ++i) {
    tmpList[i].character->advance(deltaTime);
  }
}

void DisplayList::addDisplayObject(Character* ch, int depth)
{
	ch->setDepth(depth);
	
	DisplayObjectInfo di;
	di.setCharacter(ch);
	
	int index = findIndexByDepth(depth);
	_displayObjectArray.insert(_displayObjectArray.begin()+index, di);
}

int DisplayList::findIndexByDepth(int depth)
{
	
}
