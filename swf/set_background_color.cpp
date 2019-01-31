#include "stdafx.h"
#include "set_background_color.h"
//#include "character.h"

void SetBackgroundColor::read(Stream* in)
{
	_color.readRGB(in);
}

// void SetBackgroundColor::execute(Character* m)
// {
// 	float	currentAlpha = m->getBackgroundAlpha();
// 	_color.setA(Util::frnd(currentAlpha * 255.0f));
	
// 	m->setBackgroundColor(_color);
// }

// void SetBackgroundColor::executeState(Character* m)
// {
// 	execute(m);
// }
