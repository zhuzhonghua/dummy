#include "stdafx.h"

#include "tag_loader.h"
#include "set_background_color.h"
//#include "movie_definition_sub.h"
//#include "image.h"
//#include "shape_character_def.h"

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
    printf("unimplemented tagtype %d\n", info.tagType);
    break;
  }
}

void TagLoader::endLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
	
}

void TagLoader::setBackgroundColorLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  SetBackgroundColor* t = new SetBackgroundColor();
  t->read(in);
  // m->addExecuteTag(t);
}

void TagLoader::defineBitsJPEG2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  ASSERT(info.tagType == Tag::DEFINEBITSJPEG2);
  UInt16 chId = in->readUI16();
  std::printf("definebitsjpeg2loader chid=%d\n", chId);

  // BitmapInfo* bi=NULL;
  // StreamAdapter sa(in->getUnderlyingStream(), in->getTagEndPosition());
  // Image::RGB* im = Image::readSWFJPEG2(&sa);

  // // TODO: create render bi
  // // bi = render::createbitmapinfo()
	
  // BitmapCharacter* ch = new BitmapCharacter(m, bi);
  //m->addBitmapCharacter(chId, ch);
}

void TagLoader::defineShapeLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  ASSERT(info.tagType == Tag::DEFINESHAPE || info.tagType == Tag::DEFINESHAPE2 ||
         info.tagType == Tag::DEFINESHAPE3 || info.tagType == Tag::DEFINESHAPE4);

  Uint16	chId = in->readUI16();
  std::printf("shape_loader: id = %d\n", chId);

  // ShapeCharacterDef* ch = new ShapeCharacterDef();
  // ch->read(in, info.tagType, true, m);

  // m->addCharacter(chId, ch);
}

void TagLoader::placeObject2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  ASSERT(info.tagType == Tag::PLACEOBJECT2 || info.tagType == Tag::PLACEOBJECT ||
         info.tagType == Tag::PLACEOBJECT3);

  
}
