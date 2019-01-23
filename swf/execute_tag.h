#pragma once

class Character;

class ExecuteTag
{
public:
	virtual ~ExecuteTag() {}
	virtual void	execute(Character* m) {}
	virtual void	executeState(Character* m) {}
	virtual void	executeStateReverse(Character* m, int frame) { executeState(m); }
	virtual bool	isRemoveTag() const { return false; }
	virtual bool	isActionTag() const { return false; }
	virtual UInt32	getDepthIdOfReplaceOrAddTag() const { return static_cast<UInt32>(-1); }
};

