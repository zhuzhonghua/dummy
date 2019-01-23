#pragma once

#include <boost/noncopyable.hpp>
#include <map>

#include "stream.h"

class MovieDefinitionSub;


class TagLoader : boost::noncopyable
{
public:
	static TagLoader*	get();
	void loadTag(Stream* input, const TagInfo& info, MovieDefinitionSub* m);
protected:
	TagLoader();
protected:
	typedef void(*LoaderFunction)(Stream* input, const TagInfo& info, MovieDefinitionSub* m);
	
	static void endLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
	static void setBackgroundColorLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
	static void defineShapeLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
	static void defineBitsJPEG2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
	static void placeObject2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
protected:
	LoaderFunction		getLoader(int tag);
	void addLoader(int tag_type, LoaderFunction lf);
private:
	std::map<int, LoaderFunction>	_loaders;
	static TagLoader* _inst;
};

