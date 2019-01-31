#include "stdafx.h"
#include "file.h"

File::File(const char* path)
{
	_rwops = SDL_RWFromFile(path, "rb");
}

File::~File()
{
	
}

int		File::getPosition() const
{
	return SDL_RWtell(_rwops);
}

void  File::setPosition(int pos)
{
	SDL_RWseek(_rwops, pos, RW_SEEK_SET);
}

int 	File::readBytes(void* bytes, int numBytes)
{
	return SDL_RWread(_rwops, bytes, 1, numBytes);
}

// Read a 32-bit little-endian float from this file.
float	File::readFloat32()
{
  union {
    float	f;
    Uint32	i;
  } u;
  COMPILER_ASSERT(sizeof(u) == sizeof(u.i));

  u.i = readUI32();
  return u.f;
}

StreamAdapter::StreamAdapter(File* underS, int maxPos)
    :s(underS),
     startPos(underS->getPosition()),
     endPos(maxPos),
     currPos(startPos)
{
  ASSERT(endPos>=startPos);
}

// Return the number of bytes actually read.  EOF or an error would
// cause that to not be equal to "bytes".
int	StreamAdapter::readBytes(void* bytes, int numBytes)
{
	int bytesLeft = this->endPos - this->currPos;
	if (bytesLeft < numBytes) {
		if (!bytesLeft) return 0;		
		numBytes = bytesLeft;
	}
	int actuallyRead = this->s->readBytes(bytes, numBytes);
	this->currPos += actuallyRead;
	return actuallyRead;
}
