#include "stdafx.h"

#include "tag_loader.h"
#include "set_background_color.h"
#include "movie_definition_sub.h"
#include "image.h"
#include "shape_character_def.h"

TagLoader* TagLoader::_inst = NULL;

TagLoader* TagLoader::get()
{
	if (!_inst)
	{
		_inst = new TagLoader();
	}
	return _inst;
}

TagLoader::LoaderFunction TagLoader::getLoader(int tag)
{
	std::map<int, LoaderFunction>::iterator iter=_loaders.find(tag);
	if (iter != _loaders.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
}

void TagLoader::loadTag(Stream* input, const TagInfo& info, MovieDefinitionSub* m)
{
	LoaderFunction lf = this->getLoader(info.tagType);
	if (lf)
	{
		(*lf)(input, info, m);
	}
  else
  {
    std::printf("no func to load tag %d\n", info.tagType);
  }
}

void TagLoader::addLoader(int tag, LoaderFunction lf)
{
	_loaders[tag] = lf;
}

TagLoader::TagLoader()
{
	addLoader(Tag::END,					endLoader);
	addLoader(Tag::SETBACKGROUNDCOLOR,	setBackgroundColorLoader);
	addLoader(Tag::DEFINEBITSJPEG2, defineBitsJPEG2Loader);
	addLoader(Tag::DEFINESHAPE, defineShapeLoader);
	addLoader(Tag::PLACEOBJECT2, placeObject2Loader);
}

void TagLoader::endLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
	
}

void TagLoader::setBackgroundColorLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
	SetBackgroundColor* t = new SetBackgroundColor();
	t->read(in);
	m->addExecuteTag(t);
}

void TagLoader::defineBitsJPEG2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
	ASSERT(info.tagType == Tag::DEFINEBITSJPEG2);
	UInt16 chId = in->readUI16();
  std::printf("definebitsjpeg2loader chid=%d\n", chId);

	BitmapInfo* bi=NULL;
	StreamAdapter sa(in->getUnderlyingStream(), in->getTagEndPosition());
	Image::RGB* im = Image::readSWFJPEG2(&sa);

	// TODO: create render bi
	// bi = render::createbitmapinfo()
	
	BitmapCharacter* ch = new BitmapCharacter(m, bi);
	m->addBitmapCharacter(chId, ch);
}

void TagLoader::defineShapeLoader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  ASSERT(info.tagType == Tag::DEFINESHAPE || info.tagType == Tag::DEFINESHAPE2 ||
         info.tagType == Tag::DEFINESHAPE3 || info.tagType == Tag::DEFINESHAPE4);

  Uint16	chId = in->readUI16();
  std::printf("shape_loader: id = %d\n", chId);

  ShapeCharacterDef* ch = new ShapeCharacterDef();
  ch->read(in, info.tagType, true, m);

  m->addCharacter(chId, ch);
}

void TagLoader::placeObject2Loader(Stream* in, const TagInfo& info, MovieDefinitionSub* m)
{
  ASSERT(info.tagType == Tag::PLACEOBJECT2 || info.tagType == Tag::PLACEOBJECT
         info.tagType == Tag::PLACEOBJECT3);

  
}
