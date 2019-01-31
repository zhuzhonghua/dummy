#pragma once

#include <boost/noncopyable.hpp>
#include <map>

#include "stream.h"

class MovieDefinitionSub;


class TagLoader
{
public:
  static void loadTag(Stream* input, const TagInfo& info, MovieDefinitionSub* m);
protected:
  typedef void(*LoaderFunction)(Stream* input, const TagInfo& info, MovieDefinitionSub* m);
	
  static void endLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
  static void setBackgroundColorLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
  static void defineShapeLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
  static void defineBitsJPEG2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
  static void placeObject2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m);
};

