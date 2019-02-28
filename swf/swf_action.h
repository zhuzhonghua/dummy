#pragma once

#include "def.h"

// Base class for actions.
class ActionBuffer
{
public:
  ActionBuffer();
  void	read(Stream* in);

  void operator=(const ActionBuffer& ab);
		
private:
};
