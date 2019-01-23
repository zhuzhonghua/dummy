#pragma once

#include "regex/pcrecpp.h"

namespace Util{
	template <typename T>
	const T Min(const T a, const T b)
	{
		return (a < b) ? a : b;
	}
	
	template <typename T>
	const T Max(const T a, const T b)
	{
		return (a < b) ? b : a;
	}

  template <class T>
  void swap(T* a, T* b)
  {
    T temp(*a);
    *a = *b;
    *b = temp;
  }

	inline int		fchop(float f) { return (int)f; }	// replace w/ inline asm if desired
	inline int		frnd(float f) { return fchop(f + 0.5f); }	// replace with inline asm if desired
	inline float	flerp(float a, float b, float f) { return (b - a) * f + a; }
	inline int		iclamp(int i, int min, int max) 
	{
		ASSERT(min <= max);
		return Max(min, Min(i, max));
	}

	inline float	fclamp(float f, float xmin, float xmax) {
		ASSERT(xmin <= xmax);
		return Max(xmin, Min(f, xmax));
	}

	inline float sqr(float number) {
		long i;
		float x, y;
		const float f = 1.5F;
		x = number * 0.5F;
		y = number;
		i = *(long *)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float *)&i;
		y = y * (f - (x * y * y));
		y = y * (f - (x * y * y));
		return number * y;
	}

  // Computes a hash of the given data buffer.
  // Hash function suggested by http://www.cs.yorku.ca/~oz/hash.html
  // Due to Dan Bernstein.  Allegedly very good on strings.
  //
  // One problem with this hash function is that e.g. if you take a
  // bunch of 32-bit ints and hash them, their hash values will be
  // concentrated toward zero, instead of randomly distributed in
  // [0,2^32-1], because of shifting up only 5 bits per byte.
	inline size_t	bernsteinHash(const void* data_in, int size, unsigned int seed = 5381)
	{
		const unsigned char*	data = (const unsigned char*)data_in;
		unsigned int	h = seed;
		while (size > 0) {
			size--;
			h = ((h << 5) + h) ^ (unsigned)data[size];
		}

		return h;
	}

  // Alternative: "sdbm" hash function, suggested at same web page
  // above, http::/www.cs.yorku.ca/~oz/hash.html
  //
  // This is somewhat slower, but it works way better than the above
  // hash function for hashing large numbers of 32-bit ints.
  inline size_t	sdbmHash(const void* data_in, int size, unsigned int seed = 5381)
  {
    const unsigned char*	data = (const unsigned char*) data_in;
    unsigned int	h = seed;
    while (size > 0) {
      size--;
      h = (h << 16) + (h << 6) - h + (unsigned) data[size];
    }

    return h;
  }

  // Computes a hash of the given data buffer; does tolower() on each
  // byte.  Hash function suggested by
  // http://www.cs.yorku.ca/~oz/hash.html Due to Dan Bernstein.
  // Allegedly very good on strings.
  inline size_t	bernsteinHashCaseInsensitive(const void* data_in, int size, unsigned int seed = 5381)      
  {
    const unsigned char*	data = (const unsigned char*) data_in;
    unsigned int	h = seed;
    while (size > 0) {
      size--;
      h = ((h << 5) + h) ^ (unsigned) tolower(data[size]);
    }

    // Alternative: "sdbm" hash function, suggested at same web page above.
    // h = 0;
    // for bytes { h = (h << 16) + (h << 6) - hash + *p; }

    return h;
  }

  inline int p2(int n)
  {
    int	p = 1; while (p < n) { p <<= 1; }

    // There is no sense to do 2048 from 1025
    // it is better to take 1024 instead of 1025, for example
    if ((float) n / (float) p < 0.6f)
    {
      p >>= 1;
    }
    return p;
  }
	
	void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
	void splitStringRe(const std::string& s, std::vector<std::string>& v, pcrecpp::RE re);
	void splitUTF8String(const std::string& s, std::vector<Uint16>& chs);
	void splitUTF8(const std::string& s, std::vector<std::string>& chs);
	
	std::string format(const char* format, ...);
};
