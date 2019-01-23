#include "stdafx.h"
#include "roguegame.h"
#include "zengine.h"

int main(int argc, char** argv)
{
	RogueGame dp;
  Zengine::init(&dp);
  
	dp.init();
	dp.run();
	
	return 0;
}

