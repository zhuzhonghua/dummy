#include "stdafx.h"

#include "tag_loader.h"
#include "set_background_color.h"
#include "movie_definition_sub.h"
#include "image.h"
#include "character_def.h"
#include "render/render.h"
#include "shape_character_def.h"
#include "place_object.h"

void TagLoader::loadTag(Stream* input, const TagInfo& info, MovieDefinitionSub* m)
{
  switch(info.tagType)
  {
  case Tag::END:
    endLoader(input, info, m);
    break;
  case Tag::SETBACKGROUNDCOLOR:
    setBackgroundColorLoader(input, info, m);
    break;
  case Tag::DEFINEBITSJPEG2:
    defineBitsJPEG2Loader(input, info, m);
    break;
   case Tag::DEFINESHAPE:
     defineShapeLoader(input, info, m);
     break;
   case Tag::PLACEOBJECT2:
     placeObject2Loader(input, info, m);
     break;
  default:
    ERROR("unimplemented tagtype %d", info.tagType);
    break;
  }
}

void TagLoader::endLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  INFO("tag end load tagType=%d", info.tagType);
}

void TagLoader::setBackgroundColorLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  INFO("set background color load");
  SetBackgroundColor t;// = new SetBackgroundColor();
  t.read(in);
  m->setBackgroundColor(t.getColor());
  //m->addExecuteTag(t);
}

void TagLoader::defineBitsJPEG2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  ASSERT(info.tagType == Tag::DEFINEBITSJPEG2);
  UInt16 chId = in->readUI16();
  std::printf("definebitsjpeg2 load chid=%d\n", chId);

  BitmapInfo* bi=NULL;
  StreamAdapter sa(in->getUnderlyingStream(), in->getTagEndPosition());
  Image::RGB* im = Image::readSWFJPEG2(&sa);

  bi = Render::createBitmapInfoRGB(im);
	
  BitmapCharacter* ch = new BitmapCharacter(m, bi);
  m->addBitmapCharacter(chId, ch);
}

void TagLoader::defineShapeLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  ASSERT(info.tagType == Tag::DEFINESHAPE || info.tagType == Tag::DEFINESHAPE2 ||
         info.tagType == Tag::DEFINESHAPE3 || info.tagType == Tag::DEFINESHAPE4);
	
  Uint16	chId = in->readUI16();
  INFO("shape_loader: id = %d", chId);
	
  ShapeCharacterDef* ch = new ShapeCharacterDef();
  ch->read(in, info.tagType, true, m);
	
  m->addCharacter(chId, ch);
}

void TagLoader::placeObject2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  ASSERT(info.tagType == Tag::PLACEOBJECT2 || info.tagType == Tag::PLACEOBJECT ||
         info.tagType == Tag::PLACEOBJECT3);

  INFO("placeobject loader tagtype=%d", info.tagType);
  
  PlaceObject* tag = new PlaceObject();
  tag->read(in, info.tagType, m->getVersion());
  m->addExecuteTag(tag);
}
