#include "stdafx.h"

#include "character_def.h"
#include "render/render.h"
#include "character.h"

CharacterDef::CharacterDef()
	:_chInst(NULL)
{
}

CharacterDef::~CharacterDef()
{
	if(_chInst != NULL)
	{
		//*Attention*
		//if _chInst is put to displaylist, then make sure it is removed first
		delete _chInst;
		_chInst = NULL;
	}
}

Character* CharacterDef::getCharacterInst(Character* ch, int id)
{
	if (_chInst == NULL)
	{
		_chInst = createCharacterInst(ch, id);
	}

	return _chInst;
}

Character* CharacterDef::createCharacterInst(Character* parent, int id)
{
	return new GenericCharacter(this, parent, id);
}

BitmapCharacter::BitmapCharacter(MovieDefinition* rdef, BitmapInfo* bi)
    :_bitmapInfo(bi)
{
  static int sTotal = 0;
  int w = bi->getWidth();
  int h = bi->getHeight();

  // log bitmap info
  sTotal += (Util::p2(w) * Util::p2(h)) >> 10;		// KB
  //std::printf("bitmap : w=%d, h=%d, p2w=%d, p2h=%d, mem=%dK, total=%dK\n",
  //            w, h, Util::p2(w), Util::p2(h), (Util::p2(w) * Util::p2(h)) >> 10, sTotal);
}

// Return true if the specified point is on the interior of our shape.
// Incoming coords are local coords.
bool BitmapCharacter::pointTestLocal(float x, float y)
{
  RECT coords;
  coords._x_min = 0.0f;
  coords._x_max = PIXELS_TO_TWIPS(_bitmapInfo->getWidth());
  coords._y_min = 0.0f;
  coords._y_max = PIXELS_TO_TWIPS(_bitmapInfo->getHeight());
  if (coords.pointTest(x, y))
  {
    return true;
  }
  return false;
}

void BitmapCharacter::getBound(RECT* bound)
{
  bound->_x_min = 0.0f;
  bound->_x_max = PIXELS_TO_TWIPS(_bitmapInfo->getWidth());
  bound->_y_min = 0.0f;
  bound->_y_max = PIXELS_TO_TWIPS(_bitmapInfo->getHeight());
}

void BitmapCharacter::display(Character* ch)
{
  RECT coords;
  coords._x_min = 0.0f;
  coords._x_max = PIXELS_TO_TWIPS(_bitmapInfo->getWidth());
  coords._y_min = 0.0f;
  coords._y_max = PIXELS_TO_TWIPS(_bitmapInfo->getHeight());

  // show whole picture
  RECT uvCoords;
  uvCoords._x_min = 0.0f;
  uvCoords._x_max = 1.0f;
  uvCoords._y_min = 0.0f;
  uvCoords._y_max = 1.0f;

  // TODO:
	// DONE
  //std::printf("display bitmapinfo\n");

	Render::displayBitmapInfo(_bitmapInfo);
}
