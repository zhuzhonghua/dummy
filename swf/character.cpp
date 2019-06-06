#include "stdafx.h"

#include "character.h"
#include "character_def.h"

Character::Character(Character* parent, int id)
	:_parent(parent),
	 _id(id),
	 _depth(-1)
{
	
}

GenGenericCharacter::GenericCharacter(CharacterDef* def, Character* parent, int id)
	:Character(parent, id),
	 _def(def)
{
}

void GenericCharacter::display()
{
	_def->display(this);
}
