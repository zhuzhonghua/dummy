#include "stdafx.h"

#include "swf_action.h"

ActionBuffer::ActionBuffer()
{
}

void ActionBuffer::read(Stream* in)
{
  // Read action bytes.
  for (;;)
  {
    int	action_id = in->readU8();

    if (action_id & 0x80)
    {
      // Action contains extra data.  Read it.
      int	length = in->readU16();
      for (int i = 0; i < length; i++)
      {
        Uint8	b = in->readU8();
      }
    }

    if (action_id == 0)
    {
      // end of action buffer.
      break;
    }
  }
}
