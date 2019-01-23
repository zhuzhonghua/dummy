#pragma once

class File{
public:
	File(const char* path);
	File(){ _rwops = NULL; }
	~File();
	virtual int		getPosition() const;
	virtual void  setPosition(int pos);
	virtual int 	readBytes(void* bytes, int numBytes);

  // Read a 32-bit little-endian float from this file.
  float		readFloat32();
	UInt32	readUI32() { UInt32 u; readBytes(&u, 4); return u; }
	UInt16	readUI16() { UInt16 u; readBytes(&u, 2); return u; }
	UInt8		readUI8() { UInt8 u; readBytes(&u, 1);; return u; }
  Byte		readByte() { Byte u; readBytes(&u, 1); return u; }
private:
	SDL_RWops*	_rwops;
};


// TagLater: 
class StreamAdapter : public File
{
public:
	StreamAdapter(File* underS, int maxPos);
  virtual int 	readBytes(void* bytes, int numBytes);
  
	File* s;
	int startPos;
	int endPos;
	int currPos;
};
