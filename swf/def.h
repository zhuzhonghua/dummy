#pragma once

// Unique id of all gameswf resources
enum ASClasses
  {
    AS_OBJECT,
    AS_CHARACTER,
    AS_SPRITE,
    AS_FUNCTION,
    AS_C_FUNCTION,
    AS_S_FUNCTION,
    AS_3_FUNCTION,	// action script 3 function
    AS_MOVIE_DEF,
    AS_MOVIE_DEF_SUB,
    AS_CHARACTER_DEF,
    AS_SPRITE_DEF,
    AS_VIDEO_DEF,
    AS_SOUND_SAMPLE,
    AS_VIDEO_INST,
    AS_KEY,
    AS_ARRAY,
    AS_COLOR,
    AS_SOUND,
    AS_FONT,
    AS_CANVAS,
    AS_NETSTREAM,
    AS_STRING,
    AS_SELECTION,
    AS_POINT,
    AS_MATRIX,
    AS_TRANSFORM,
    AS_COLOR_TRANSFORM,
    AS_NETCONNECTION,
    AS_LISTENER,
    AS_DATE,
    AS_EDIT_TEXT,
    AS_XML_SOCKET,
    AS_TEXTFORMAT,
    AS_MCLOADER,
    AS_LOADVARS,
    AS_TIMER,
    AS_MOUSE,

    // flash9
    AS_EVENT,
    AS_MOUSE_EVENT,

    // plugins
    AS_PLUGIN_MYDB,
    AS_PLUGIN_MYTABLE,
    AS_PLUGIN_3DS,
    AS_PLUGIN_FILE,
    AS_PLUGIN_SQLITE_DB,
    AS_PLUGIN_SQLITE_TABLE,
    AS_PLUGIN_SQL_DB,
    AS_PLUGIN_SQL_TABLE,

    // user defined plugins
    // should be the last in this enum
    AS_USER_PLUGIN = 1000
  };

enum Tag
  {
    END = 0,
    SHOWFRAME = 1,
    DEFINESHAPE = 2,
		
    PLACEOBJECT = 4,
    REMOVEOBJECT = 5,
    DEFINEBITS = 6,
    DEFINEBUTTON = 7,
    JPEGTABLES = 8,
    SETBACKGROUNDCOLOR = 9,
    DEFINEFONT = 10,
    DEFINETEXT = 11,
    DOACTION = 12,
    DEFINEFONTINFO = 13,
    DEFINESOUND = 14,
    STARTSOUND = 15,
    STOPSOUND = 16,
    DEFINEBUTTONSOUND = 17,
    SOUNDSTREAMHEAD = 18,
    SOUNDSTREAMBLOCK = 19,
    DEFINELOSSLESS = 20,
    DEFINEBITSJPEG2 = 21,
    DEFINESHAPE2 = 22,
    DEFINEBUTTONCXFORM = 23,
    PROTECT = 24,

    PLACEOBJECT2 = 26,

    REMOVEOBJECT2 = 28,
		
    DEFINESHAPE3 = 32,
    DEFINETEXT2 = 33,
    DEFINEBUTTON2 = 34,
    DEFINEBITSJPEG3 = 35,
    DEFINELOSSLESS2 = 36,
    DEFINEEDITTEXT = 37,

    DEFINESPRITE = 39,

    FRAMELABEL = 43,

    SOUNDSTREAMHEAD2 = 45,
    DEFINEMORPHSHAPE = 46,

    DEFINEFONT2 = 48,

    EXPORTASSETS = 56,
    IMPORTASSETS = 57,
    ENABLEDEBUGGER = 58,
    INITACTION = 59,
    DEFINEVIDEOSTREAM = 60,
    VIDEOFRAME = 61,
    DEFINEFONTINFO2 = 62,

    ENABLEDEBUGGER2 = 64,
    SCRIPTLIMITS = 65,
    SETTABINDEX = 66,

    FILEATTRIBUTES = 69,
    PLACEOBJECT3 = 70,
    IMPORTASSETS2 = 71,

    DEFINEALIGNZONES = 73,
    CSMTEXTSETTINGS = 74,
    DEFINEFONT3 = 75,
    SYMBOLCLASS = 76,
    METADATA = 77,
    DEFINESCALINGGRID = 78,

    DOABCDEFINE = 82,
    DEFINESHAPE4 = 83,
    DEFINEMORPHSHAPE2 = 84,

    DEFINESCENEANDFRAMELABELDATA = 86,
    DEFINEBINARYDATA = 87,
    DEFINEFONTNAME = 88,
    STARTSOUND2 = 89,
  };

// This is the base class for all ActionScript-able objects
// ("as_" stands for ActionScript).
class ASObjectInterface
{
public:
  virtual bool is(int class_id) const = 0;

  virtual ~ASObjectInterface() {}
};


class BitmapInfo{
public:
  virtual void layout() {};
  virtual void activate() {};
  virtual int getWidth() const { return 0; }
  virtual int getHeight() const { return 0; }
  virtual unsigned char* getData() const { return 0; }
  virtual int getBPP() const { return 0; }	// byte per pixel
};

// Keyboard handling
namespace Key
{
enum Code
  {
    INVALID = 0,
    A = 65,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    _0 = 48,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    KP_0 = 96,
    KP_1,
    KP_2,
    KP_3,
    KP_4,
    KP_5,
    KP_6,
    KP_7,
    KP_8,
    KP_9,
    KP_MULTIPLY,
    KP_ADD,
    KP_ENTER,
    KP_SUBTRACT,
    KP_DECIMAL,
    KP_DIVIDE,
    F1 = 112,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    BACKSPACE = 8,
    TAB,
    CLEAR = 12,
    ENTER,
    SHIFT = 16,
    CONTROL,
    ALT,
    CAPSLOCK = 20,
    ESCAPE = 27,
    SPACE = 32,
    PGDN,
    PGUP,
    END = 35,
    HOME,
    LEFT,
    UP,
    RIGHT,
    DOWN,
    INSERT = 45,
    DELETEKEY,
    HELP,
    NUM_LOCK = 144,
    SEMICOLON = 186,
    EQUALS = 187,
    COMMA = 188,
    MINUS = 189,
    PERIOD = 190,
    SLASH = 191,
    BACKTICK = 192,
    LEFT_BRACKET = 219,
    BACKSLASH = 220,
    RIGHT_BRACKET = 221,
    QUOTE = 222,

    KEYCOUNT
  };
}	// end namespace key

class ASObject:public ASObjectInterface{
public:
  // Unique id of a gameswf resource
  enum	{ _classId = AS_OBJECT };
  virtual bool is(int classId) const
  {
    return _classId == classId;
  }
};
