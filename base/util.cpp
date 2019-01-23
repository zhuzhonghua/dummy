#include "stdafx.h"

#include "util.h"

namespace Util{
void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void splitStringRe(const std::string& s, std::vector<std::string>& v, pcrecpp::RE re)
{
	pcrecpp::StringPiece input(s);
	const char* lastP = input.data();

	std::string match;
	while (re.Consume(&input, &match))
	{
		const char* p = input.data();
		int range = p - lastP;
		v.push_back(std::string(lastP, lastP + range));

		lastP += range;
		lastP += match.length();
	}
	v.push_back(std::string(lastP));
}

/* Gets a unicode value from a UTF-8 encoded string and advance the string */
#define UNKNOWN_UNICODE 0xFFFD
static Uint32 UTF8_getch(const char **src, size_t *srclen)
{
	const Uint8 *p = *(const Uint8**)src;
	size_t left = 0;
	SDL_bool overlong = SDL_FALSE;
	SDL_bool underflow = SDL_FALSE;
	Uint32 ch = UNKNOWN_UNICODE;

	if (*srclen == 0) {
		return UNKNOWN_UNICODE;
	}
	if (p[0] >= 0xFC) {
		if ((p[0] & 0xFE) == 0xFC) {
			if (p[0] == 0xFC && (p[1] & 0xFC) == 0x80) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x01);
			left = 5;
		}
	}
	else if (p[0] >= 0xF8) {
		if ((p[0] & 0xFC) == 0xF8) {
			if (p[0] == 0xF8 && (p[1] & 0xF8) == 0x80) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x03);
			left = 4;
		}
	}
	else if (p[0] >= 0xF0) {
		if ((p[0] & 0xF8) == 0xF0) {
			if (p[0] == 0xF0 && (p[1] & 0xF0) == 0x80) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x07);
			left = 3;
		}
	}
	else if (p[0] >= 0xE0) {
		if ((p[0] & 0xF0) == 0xE0) {
			if (p[0] == 0xE0 && (p[1] & 0xE0) == 0x80) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x0F);
			left = 2;
		}
	}
	else if (p[0] >= 0xC0) {
		if ((p[0] & 0xE0) == 0xC0) {
			if ((p[0] & 0xDE) == 0xC0) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x1F);
			left = 1;
		}
	}
	else {
		if ((p[0] & 0x80) == 0x00) {
			ch = (Uint32)p[0];
		}
	}
	++*src;
	--*srclen;
	while (left > 0 && *srclen > 0) {
		++p;
		if ((p[0] & 0xC0) != 0x80) {
			ch = UNKNOWN_UNICODE;
			break;
		}
		ch <<= 6;
		ch |= (p[0] & 0x3F);
		++*src;
		--*srclen;
		--left;
	}
	if (left > 0) {
		underflow = SDL_TRUE;
	}
	/* Technically overlong sequences are invalid and should not be interpreted.
	However, it doesn't cause a security risk here and I don't see any harm in
	displaying them. The application is responsible for any other side effects
	of allowing overlong sequences (e.g. string compares failing, etc.)
	See bug 1931 for sample input that triggers this.
	*/
	/*if (overlong) return UNKNOWN_UNICODE;*/
	if (underflow ||
		(ch >= 0xD800 && ch <= 0xDFFF) ||
		(ch == 0xFFFE || ch == 0xFFFF) || ch > 0x10FFFF) {
		ch = UNKNOWN_UNICODE;
	}
	return ch;
}

void splitUTF8String(const std::string& s, std::vector<Uint16>& chs)
{
	const char* text = s.c_str();
	size_t textlen = SDL_strlen(text);
	while (textlen > 0) 
	{
		Uint16 c = UTF8_getch(&text, &textlen);
		if (c == UNICODE_BOM_NATIVE || c == UNICODE_BOM_SWAPPED) 
		{
			continue;
		}
		chs.push_back(c);
	}
}

void splitUTF8(const std::string& s, std::vector<std::string>& chs)
{
	for (int i = 0; s[i] != '\0';)
	{
		char chr = s[i];
		//chrÊÇ0xxx xxxx£¬¼´asciiÂë  
		if ((chr & 0x80) == 0)
		{
			chs.push_back(s.substr(i, 1));
			++i;
		}//chrÊÇ1111 1xxx  
		else if ((chr & 0xF8) == 0xF8)
		{
			chs.push_back(s.substr(i, 5));			
			i += 5;
		}//chrÊÇ1111 xxxx  
		else if ((chr & 0xF0) == 0xF0)
		{
			chs.push_back(s.substr(i, 4));
			i += 4;
		}//chrÊÇ111x xxxx  
		else if ((chr & 0xE0) == 0xE0)
		{
			chs.push_back(s.substr(i, 3));
			i += 3;
		}//chrÊÇ11xx xxxx  
		else if ((chr & 0xC0) == 0xC0)
		{
			chs.push_back(s.substr(i, 2));
			i += 2;
		}
		else
		{
			// Bad UTF8
			*(int*)0 = 0;
		}
	}
}

std::string format(const char* format, ...)
{
	char buf[256] = { 0 };
	
	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	return buf;
}
};
