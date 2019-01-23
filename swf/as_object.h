#pragma once

#include "def.h"

class ASObject:public ASObjectInterface{
public:
	// Unique id of a gameswf resource
	enum	{ _classId = AS_OBJECT };
	virtual bool is(int classId) const
	{
		return _classId == classId;
	}
};
