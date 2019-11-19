#include "stdafx.h"

#include "character.h"
#include "character_def.h"

Character::Character(Character* parent, int id)
	:_parent(parent),
	 _id(id),
	 _depth(-1)
{
	
}

// Get our concatenated matrix (all our ancestor transforms, times our matrix).	 Maps
// from our local space into "world" space (i.e. root movie space).
Matrix Character::getWorldMatrix() const
{
	const Character * current = _parent;

	Matrix m = getMatrix();

	// TODO: pre calculate
	while (current != NULL)
	{
		Matrix new_m;

		new_m = current->getMatrix();
		new_m.concatenate(m);
		m = new_m;
		current = current->_parent;
	}

	return m;
}

void Character::getBound(RECT* bound)
{
	CharacterDef* def = getCharacterDef();
	ASSERT(def);
	def->getBound(bound);
	getMatrix().transform(bound);
}

GenericCharacter::GenericCharacter(CharacterDef* def, Character* parent, int id)
	:Character(parent, id),
	 _def(def)
{
}

void GenericCharacter::display()
{
	_def->display(this);
}
