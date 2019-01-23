#include "stdafx.h"

#include "membuf.h"


// Allocate in increments of BLOCKSIZE.
static const int BLOCKSIZE = (1 << 12);

static int capacity(int size)
// Compute the buffer capacity corresponding to the given size.
// Basically round up to the next block size.
// Always return non-zero.
{
	// BLOCKSIZE must be a power of two.
	COMPILER_ASSERT((BLOCKSIZE & (BLOCKSIZE - 1)) == 0);

	if (size == 0) {
		// Special case, always allocate.
		return BLOCKSIZE;
	}

	return (size + BLOCKSIZE - 1) & ~(BLOCKSIZE - 1);
}


membuf::membuf() :
	m_size(0),
	m_capacity(0),
	m_data(0),
	m_read_only(false)
{
}


membuf::membuf(const void* data, int size) :
	m_size(0),
	m_capacity(0),
	m_data(0),
	m_read_only(false)
{
	append(data, size);
}


membuf::membuf(const membuf& buf) :
	m_size(0),
	m_capacity(0),
	m_data(0),
	m_read_only(false)
{
	append(buf);
}


membuf::membuf(const tu_string& str) :
	m_size(0),
	m_capacity(0),
	m_data(0),
	m_read_only(false)
{
	append(str);
}


// Special read-only constructor.
membuf::membuf(read_only_enum e, const void* data, int size) :
	m_size(size),
	m_capacity(0),
	m_data(const_cast<void*>(data)),
	m_read_only(true)
{
}


membuf::~membuf()
{
	if (!m_read_only && m_capacity) 
	{
		free(m_data, m_capacity);
	}
	m_data = NULL;
}


void membuf::resize(int new_size)
{
	ASSERT(!m_read_only);

	if (new_size == m_size)
	{
		return;
	}

	int new_capacity = capacity(new_size);

	if (m_data == NULL)
	{
		m_data = malloc(new_capacity);
	}
	else
	{
		if (new_capacity != m_capacity) 
		{
			m_data = realloc(m_data, new_capacity, m_capacity);
		}
	}

	ASSERT(m_data);

	m_capacity = new_capacity;
	assert(m_capacity >= new_size);
	m_size = new_size;
}


void membuf::append(const void* data, int datasize)
{
	assert(m_read_only == false);

	int old_size = size();
	resize(old_size + datasize);
	memcpy(((char*) m_data) + old_size, data, datasize);
}


void membuf::append(const membuf& buf)
{
	append(buf.data(), buf.size());
}


void membuf::append(const tu_string& str)
{
	append(str.c_str(), str.length());
}

Uint8&	membuf::operator[](int index)
{
	ASSERT(index >= 0 && index < m_size); 
	return ((Uint8*) m_data)[index]; 
}

const Uint8&	membuf::operator[](int index) const
{
	ASSERT(index >= 0 && index < m_size); 
	return ((Uint8*) m_data)[index]; 
}

void membuf::operator=(const membuf& buf)
{
	resize(buf.size());
	memcpy(m_data, buf.m_data, size());
	m_read_only = buf.m_read_only;
}

bool	membuf::operator==(const membuf& buf) const
{
	if (size() != buf.size())
	{
		return false;
	}
	return memcmp(m_data, buf.m_data, size()) == 0 ? true : false;
}

bool	membuf::operator!=(const membuf& buf) const
{
	if (size() != buf.size())
	{
		return false;
	}
	return memcmp(m_data, buf.m_data, size()) == 0 ? false : true;
}

void membuf::append(Uint8 byte)
{
	ASSERT(m_read_only == false);

	int old_size = size();
	resize(old_size + 1);
	((Uint8*) m_data)[old_size] = byte;
}
