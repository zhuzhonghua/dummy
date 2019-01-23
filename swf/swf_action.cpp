#include "stdafx.h"

#include "swf_action.h"

ActionBuffer::ActionBuffer() :
		_buffer(new CountedBuffer),
		_declDictProcessedAt(-1)
{
}

void ActionBuffer::read(Stream* in)
{
  // Read action bytes.
  for (;;)
  {

    int	instruction_start = _buffer->size();

    int	pc = _buffer->size();

    int	action_id = in->readU8();
    _buffer->append((Uint8) action_id);

    if (action_id & 0x80)
    {
      // Action contains extra data.  Read it.
      int	length = in->readU16();
      _buffer->append(Uint8(length & 0x0FF));
      _buffer->append(Uint8((length >> 8) & 0x0FF));
      for (int i = 0; i < length; i++)
      {
        Uint8	b = in->readU8();
        _buffer->append(b);
      }
    }

    //IF_VERBOSE_ACTION(log_msg("%4d\t", pc); log_disasm(&(*m_buffer.get_ptr())[instruction_start]); );

    if (action_id == 0)
    {
      // end of action buffer.
      break;
    }
  }
}
