#include "stdafx.h"

#include "place_object.h"
#include "filters.h"
#include "swf_action.h"
#include "character.h"

PlaceObject::PlaceObject()
  :tagType(0), ratio(0), hasMatrix(false), hasCXForm(false), depth(0),
   characterId(0), clipDepth(0), blendMode(0), placeType(PLACE)
{
}

PlaceObject::~PlaceObject()
{
}

void PlaceObject::read(Stream* in, int tagType, int movieVersion)
{
  ASSERT(tagType == Tag::PLACEOBJECT || tagType == Tag::PLACEOBJECT2 || tagType == Tag::PLACEOBJECT3);

  this->tagType = tagType;

  if (tagType == Tag::PLACEOBJECT)
  {
    // Original place_object tag; very simple.
    characterId = in->readU16();
    depth = in->readU16();
    matrix.read(in);

    INFO("  char_id = %d\n"
	 "  depth = %d\n"
	 "  mat = \n",
	 characterId,
	 depth);
    //m_matrix.print());

    if (in->getPosition() < in->getTagEndPosition())
    {
      colorTransform.readRGB(in);
      //IF_VERBOSE_PARSE(log_msg("  cxform:\n"); m_color_transform.print());
    }
  }
  else if (tagType == Tag::PLACEOBJECT2 || tagType == Tag::PLACEOBJECT3)
  {
    in->align();

    bool	has_actions = in->readUint(1) ? true : false;
    bool	has_clip_bracket = in->readUint(1) ? true : false;
    bool	has_name = in->readUint(1) ? true : false;
    bool	has_ratio = in->readUint(1) ? true : false;
    bool	has_cxform = in->readUint(1) ? true : false;
    bool	has_matrix = in->readUint(1) ? true : false;
    bool	has_char = in->readUint(1) ? true : false;
    bool	flag_move = in->readUint(1) ? true : false;

    bool	has_image = false;
    bool	has_class_name = false;
    bool	has_cache_asbitmap = false;
    bool	has_blend_mode = false;
    bool	has_filter_list = false;

    if (tagType == Tag::PLACEOBJECT3)
    {
      in->readUint(3); // unused

      has_image = in->readUint(1) ? true : false;
      has_class_name = in->readUint(1) ? true : false;
      has_cache_asbitmap = in->readUint(1) ? true : false;
      has_blend_mode = in->readUint(1) ? true : false;
      has_filter_list = in->readUint(1) ? true : false;
    }

    depth = in->readU16();
    INFO("  depth = %d", depth);

    if (has_char)
    {
      characterId = in->readU16();
      INFO("  char id = %d", characterId);
    }

    if (has_matrix)
    {
      hasMatrix = true;
      matrix.read(in);
    }
    if (has_cxform)
    {
      hasCXForm = true;
      colorTransform.readRGBA(in);
      //IF_VERBOSE_PARSE(log_msg("  cxform:\n"); m_color_transform.print());
    }

    if (has_ratio)
    {
      ratio = (float)in->readU16() / (float)65535;
      INFO("  ratio: %f", ratio);
    }

    if (has_name)
    {
      in->readString(&characterName);
      INFO("  name = %s", characterName.c_str());
    }
    if (has_clip_bracket)
    {
      clipDepth = in->readU16(); 
      INFO("  clip_depth = %d", clipDepth);
    }

    if (has_filter_list)
    {
      SWF::readFilterList(in);
    }

    if (has_blend_mode)
    {
      // TODO, implement blend_mode
      blendMode = in->readU8();
      INFO("  blendmode = %d", blendMode);
    }

    if (has_actions)
    {
      Uint16	reserved = in->readU16();
      UNUSED(reserved);

      // The logical 'or' of all the following handlers.
      // I don't think we care about this...
      Uint32	all_flags = 0;
      if (movieVersion >= 6)
      {
        all_flags = in->readU32();
      }
      else
      {
        all_flags = in->readU16();
      }
      UNUSED(all_flags);

      INFO("  actions: flags = 0x%X", all_flags);

      // Read swf_events.
      for (;;)
      {
        // Read event.
        in->align();

        Uint32 flags = (movieVersion >= 6) ? in->readU32() : in->readU16();
        if (flags == 0)
        {
          // Done with events.
          break;
        }

        Uint32 event_length = in->readU32();
        Uint8 ch = -1;//key::INVALID;

        if (flags & (1 << 17))	// has keypress event
        {
          ch = in->readU8();
          event_length--;
        }

        // Read the actions for event(s)
        ActionBuffer action;
        action.read(in);

	// TODO: check assert
        //if (action.getLength() != static_cast<int>(event_length))
        {
          //ASSERT(0);
          //log_error("swf_event::read(), event_length = %d, but read %d\n",
          //          event_length, action.get_length());
          //break;
        }

        // 13 bits reserved, 19 bits used
        //static const EventId s_code_bits[19] =
	//  {
	//    EventId::LOAD,
	//    EventId::ENTER_FRAME,
	//    EventId::UNLOAD,
	//    EventId::MOUSE_MOVE,
	//    EventId::MOUSE_DOWN,
	//    EventId::MOUSE_UP,
	//    EventId::KEY_DOWN,
	//    EventId::KEY_UP,
	//    EventId::DATA,
	//    EventId::INITIALIZE,
	//    EventId::PRESS,
	//    EventId::RELEASE,
	//    EventId::RELEASE_OUTSIDE,
	//    EventId::ROLL_OVER,
	//    EventId::ROLL_OUT,
	//    EventId::DRAG_OVER,
	//    EventId::DRAG_OUT,
	//    EventId(EventId::KEY_PRESS, key::CONTROL),
	//    EventId::CONSTRUCT
	//  };
	//
        //// Let's see if the event flag we received is for an event that we know of
        //if (1 << (ARRAYSIZE(s_code_bits)) < flags)
        //{
        //  ASSERT(0);
        //  //log_error("swf_event::read() -- unknown event type received, flags = 0x%x\n", flags);
        //}

        //for (int i = 0, mask = 1; i < int(sizeof(s_code_bits)/sizeof(s_code_bits[0])); i++, mask <<= 1)
        //{
        //  if (flags & mask)
        //  {
        //    SWFEvent* ev = new SWFEvent;
        //    ev->event = s_code_bits[i];
	//
        //    if (i == 17)	// has keypress event ?
        //    {
        //      ev->event.keyCode = ch;
        //    }
	//
        //    // Create a function to execute the actions.
        //    array<with_stack_entry>	empty_with_stack;
        //    as_s_function*	func = new as_s_function(player, &action, 0, empty_with_stack);
        //    func->set_length(action.get_length());
	//
        //    ev->m_method.set_as_object(func);
	//
        //    m_event_handlers.push_back(ev);
        //  }
        //}
      }
    }

    if (has_char == true && flag_move == true)
    {
      // Remove whatever's at m_depth, and put m_character there.
      placeType = REPLACE;
      INFO("  placeType REPLACE");
    }
    else if (has_char == false && flag_move == true)
    {
      // Moves the object at m_depth to the new location.
      placeType = MOVE;
      INFO("  placeType MOVE");
    }
    else if (has_char == true && flag_move == false)
    {
      // Put m_character at m_depth.
      placeType = PLACE;
      INFO("  placeType PLACE");
    }

    INFO("  place object at depth %i\n", depth);
  }
}

void PlaceObject::execute(Character* ch)
{
	switch(placeType)
	{
	case PlaceType::PLACE:
		ch->addDisplayObject(characterId, depth);
		break;
		//TODO: other type
	}
}
