#pragma once

struct TagInfo
{
	int	tagType; // Tag
	int	tagOffset;
	int	tagLength;
	int	tagDataOffset;
};

class File;

class Stream
{
public:
	Stream(File* file);
	~Stream();

	int		getPosition() const;
	void  setPosition(int pos);
	int 	readBytes(void* bytes, int numBytes);

	UInt32	readUI32() { align(); UInt32 u; readBytes(&u, 4); return u; }
	UInt16	readUI16() { align(); UInt16 u; readBytes(&u, 2); return u; }
	UInt16	readU16() { align(); UInt16 u; readBytes(&u, 2); return u; }
	UInt8		readUI8() { align(); UInt8 u; readBytes(&u, 1);; return u; }
	UInt8		readU8() { align(); UInt8 u; readBytes(&u, 1);; return u; }
	SInt32	readSI32() { align(); SInt32 u; readBytes(&u, 4); return u; }
	SInt16	readSI16() { align(); SInt16 u; readBytes(&u, 2); return u; }
	SInt8		readSI8() { align(); SInt8 u; readBytes(&u, 1);; return u; }
	Byte		readByte() { return readUI8(); }
  bool		readBool() { return readUInt(1)?true:false;}
  float		readFixed();
  void 		readString(std::string* str);
  float 	readFloat();
	UInt32	readUInt(int bitCount);
  UInt32	readUint(int bitCount){ return readUInt(bitCount); }
	SInt32	readSInt(int bitCount);
	SInt32	readSint(int bitCount){ return readSInt(bitCount); }
  void		align() { _unusedBits = 0; _unusedData=0;}

	int			openTag(TagInfo* info);
	void		closeTag();

  int 		getTagEndPosition() { return _tagStack.back(); }
  File*		getUnderlyingStream() { return _file; }
private:
	File*			_file;

	UInt8			_unusedBits;
	UInt8			_unusedData;

	std::vector<int>	_tagStack;
};
