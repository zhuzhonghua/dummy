#include "stdafx.h"

#include "zlibfile.h"

ZLibFile::ZLibFile(File* file)
:_file(file),
_initialPos(file->getPosition()),
_logicalPos(0),
_atEof(false),
_error(0)
{
	_zstream.zalloc = (alloc_func)0;
	_zstream.zfree = (free_func)0;
	_zstream.opaque = (voidpf)0;

	_zstream.next_in = 0;
	_zstream.avail_in = 0;

	_zstream.next_out = 0;
	_zstream.avail_out = 0;

	int	err = inflateInit(&_zstream);
	ASSERT(err == Z_OK);
}

ZLibFile::~ZLibFile()
{
	_file = 0;
}

int	ZLibFile::getPosition() const
{
	return _logicalPos;
}

void ZLibFile::reset()
{
	_error = 0;
	_atEof = 0;
	int	err = inflateReset(&_zstream);
	if (err != Z_OK) {
		_error = 1;
		return;
	}

	_zstream.next_in = 0;
	_zstream.avail_in = 0;

	_zstream.next_out = 0;
	_zstream.avail_out = 0;

	// Rewind the underlying stream.
	_file->setPosition(_initialPos);

	_logicalPos = 0;
}

void ZLibFile::setPosition(int pos)
{
	if (_error)
	{
		return;
	}

	// If we're seeking backwards, then restart from the beginning.
	if (pos < _logicalPos)
	{
		reset();
	}

	unsigned char	temp[ZBUF_SIZE];

	// Now seek forwards, by just reading data in blocks.
	while (_logicalPos < pos)
	{
		int	toRead = pos - _logicalPos;
		int	toReadThisTime = Util::Min(toRead, ZBUF_SIZE);
		ASSERT(toReadThisTime > 0);

		int	bytesRead = readBytes(temp, toReadThisTime);
		ASSERT(bytesRead <= toReadThisTime);
		if (bytesRead == 0)
		{
			// Trouble; can't seek any further.
			break;
		}
	}

	ASSERT(_logicalPos <= pos);
}

int ZLibFile::readBytes(void* bytes, int numBytes)
{
	if (_error)
	{
		return 0;
	}
	_zstream.next_out = (unsigned char*)bytes;
	_zstream.avail_out = numBytes;

	for (;;)
	{
		if (_zstream.avail_in == 0)
		{
			// Get more raw data.
			int	new_bytes = _file->readBytes(_rawdata, ZBUF_SIZE);
			if (new_bytes == 0)
			{
				// The cupboard is bare!  We have nothing to feed to inflate().
				break;
			}
			else
			{
				_zstream.next_in = _rawdata;
				_zstream.avail_in = new_bytes;
			}
		}

		int	err = inflate(&_zstream, Z_SYNC_FLUSH);
		if (err == Z_STREAM_END)
		{
			_atEof = true;
			break;
		}
		if (err != Z_OK)
		{
			// something's wrong.
			_error = 1;
			break;
		}

		if (_zstream.avail_out == 0)
		{
			break;
		}
	}

	int	bytes_read = numBytes - _zstream.avail_out;
	_logicalPos += bytes_read;

	return bytes_read;
}
