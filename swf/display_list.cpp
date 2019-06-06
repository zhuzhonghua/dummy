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
	//TODO: if need to check the ch is already in the list
	ch->setDepth(depth);
	
	DisplayObjectInfo di;
	di.setCharacter(ch);
	
	int index = findIndexByDepth(depth);
	_displayObjectArray.insert(_displayObjectArray.begin()+index, di);

	//TODO: execute or not
	//ch->executeFrameTags(0);
}

int DisplayList::findIndexByDepth(int depth)
{
	for(int i=0; i<_displayObjectArray.size(); i++)
	{
		const DisplayObjectInfo& di = _displayObjectArray[i];
		if(depth <= di.character->getDepth())
		{
			return i;
		}
	}

	return _displayObjectArray.size();
}
