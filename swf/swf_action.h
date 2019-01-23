#pragma once

#include "swf_value.h"
#include "def.h"

//
// event_id
//
// For keyDown and stuff like that.

class EventId
{
public:
  // These must match the function names in event_id::get_function_name()
  enum IDCode
  {
    INVALID,

    // These are for buttons & sprites.
    PRESS,
    RELEASE,
    RELEASE_OUTSIDE,
    ROLL_OVER,
    ROLL_OUT,
    DRAG_OVER,
    DRAG_OUT,
    KEY_PRESS,

    // These are for sprites only.
    INITIALIZE,
    LOAD,
    UNLOAD,
    ENTER_FRAME,
    MOUSE_DOWN,
    MOUSE_UP,
    MOUSE_MOVE,
    KEY_DOWN,
    KEY_UP,
    DATA,

    CONSTRUCT,
    SETFOCUS,
    KILLFOCUS,			

    // MovieClipLoader events
    ONLOAD_COMPLETE,
    ONLOAD_ERROR,
    ONLOAD_INIT,
    ONLOAD_PROGRESS,
    ONLOAD_START,

    // sound
    ON_SOUND_COMPLETE,

    EVENT_COUNT
  };

  // 'm_unused' was added because of problem with hash<event_id, ...>
  // the reason: sizeof(unsigned char+unsigned char+character*)  == 8 
  // and sizeof(unsigned char+unsigned char+Uint16+character*)  == 8
  // I think that compiler do aligment
  unsigned char	id;
  unsigned char	keyCode;
  Uint16	unused;
  array<ASValue>* m_args;

	EventId() :id(INVALID),
      keyCode(Key::INVALID),
      unused(0),
      args(NULL)
  {
  }

	EventId(IDCode id, array<ASValue>* args) :id((unsigned char) id),
      keyCode(Key::INVALID),
      unused(0),
      args(args)
  {
  }

	EventId(IDCode id, Key::Code c = Key::INVALID) :id((unsigned char) id),
      keyCode((unsigned char) c),
      unused(0),
      args(NULL)
  {
    // For the button key events, you must supply a keycode.
    // Otherwise, don't.
    ASSERT((keyCode == Key::INVALID && (id != KEY_PRESS))
           || (keyCode != Key::INVALID && (id == KEY_PRESS)));
  }

  bool	operator==(const EventId& id) const
  {
    return id == id.id &&
    keyCode == id.keyCode &&
    args == id.args;
  }

  // Return the name of a method-handler function corresponding to this event.
  const std::string&	getFunctionName() const;
};

// allows sharing of as byte code buffer
class CountedBuffer : public membuf
{
};

// Base class for actions.
class ActionBuffer
{
public:
  ActionBuffer();
  void	read(Stream* in);
  //void	execute(ASEnvironment* env);
  //void	execute(
	//		ASEnvironment* env,
	//		int start_pc,
	//		int exec_bytes,
	//		ASValue* retval,
	//		const array<with_stack_entry>& initial_with_stack,
	//		bool is_function2) const;

  //static as_object* load_as_plugin(player* player,
  //                                 const tu_string& classname, const array<as_value>& params);
  int	getLength() const { return _buffer->size(); }
  void operator=(const ActionBuffer& ab);
		
//#if ACTION_BUFFER_PROFILLING
//  static void log_and_reset_profiling( void );
//#endif

 private:
  // Don't put these as values in array<>!  They contain
  // internal pointers and cannot be moved or copied.
  // If you need to keep an array of them, keep pointers
  // to new'd instances.
  //action_buffer(const action_buffer& a) { assert(0); }

  void	processDeclDict(int start_pc, int stop_pc);
  //static void	enumerate(as_environment* env, as_object* object);

  // data:
  CountedBuffer*	_buffer;
  array<std::string>	_dictionary;
  int	_declDictProcessedAt;
//#if ACTION_BUFFER_PROFILLING		
//  static hash<int, Uint64> profiling_table;
//#endif
};
