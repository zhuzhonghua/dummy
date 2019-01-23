#pragma once

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
