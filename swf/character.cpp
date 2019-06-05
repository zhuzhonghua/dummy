#include "stdafx.h"

#include "character.h"
#include "character_def.h"

void GenericCharacter::display()
{
	_def->display(this);
}
