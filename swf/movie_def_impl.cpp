#include "stdafx.h"

#include "movie_def_impl.h"

#include "tag_loader.h"
//#include "movie.h"
#include "sprite_instance.h"

MovieDefImpl::MovieDefImpl(SpriteInstance* sprite)
{
  _sprite = sprite;
}

bool MovieDefImpl::readHead(File* file)
{
  int	fileStartPos = file->getPosition();

  UInt8 header[4] = { 0 };
  file->readBytes(header, 4);
  ASSERT(header[0]=='C' || header[0]=='F');
  ASSERT(header[1] == 'W');
  ASSERT(header[2] == 'S');

  _version = header[3];
  int fileLen = file->readUI32();

  _fileEndPos = fileStartPos + fileLen;

  _loadedLength = 0;
	
  bool	compressed = (header[0] == 'C');
  if (compressed)
  {
    _zlibFile = new ZLibFile(file);
    _str = new Stream(_zlibFile);

    _fileEndPos = fileLen - 8;
  }
  else
  {
    _str = new Stream(file);
  }

  _loadedLength = _str->getPosition();
  
  _frameSize.read(_str);
  _frameRate = _str->readUI16() / 256.0f;

  _loadedLength = _str->getPosition();
  
  int fc = _str->readUI16();
  setFrameCount(fc);

  _loadedLength = _str->getPosition();
  
  _playList.resize(_frameCount);
  _initActionList.resize(_frameCount);

  INFO("read header %d %d %d version %d", header[0], header[1], header[2], _version);
  INFO("framecount %d _frameRate %f", _frameCount, _frameRate);

  //readTags();

  return true;
}

void MovieDefImpl::readTags()
{
  while (_str->getPosition() < _fileEndPos)
  {
    TagInfo info;
    _str->openTag(&info);

    INFO("\nread tag %d %s", info.tagType, getTagName(info.tagType));
    if (info.tagType == Tag::SHOWFRAME)
    {
      INFO("showframe tagType=%d", info.tagType);
      incLoadingFrame();
    }
    else if (info.tagType == Tag::FILEATTRIBUTES)
    {
      INFO("ignore fileattributes");
    }
    else
    {
      TagLoader::loadTag(_str, info, this);
    }
    _str->closeTag();
    //std::printf("close tag %d \n", info.tagType);
		
    _loadedLength = _str->getPosition();
  }
}


#define GET_TAG_NAME(tag) case Tag::tag: return #tag;

char* MovieDefImpl::getTagName(int tag)
{
	switch(tag)
	{
		GET_TAG_NAME(END);
		GET_TAG_NAME(SHOWFRAME);
		GET_TAG_NAME(DEFINESHAPE);

		GET_TAG_NAME(PLACEOBJECT);
		GET_TAG_NAME(REMOVEOBJECT);
		GET_TAG_NAME(DEFINEBITS);
		GET_TAG_NAME(DEFINEBUTTON);
		GET_TAG_NAME(JPEGTABLES);
		GET_TAG_NAME(SETBACKGROUNDCOLOR);
		GET_TAG_NAME(DEFINEFONT);
		GET_TAG_NAME(DEFINETEXT);
		GET_TAG_NAME(DOACTION);
		GET_TAG_NAME(DEFINEFONTINFO);
		GET_TAG_NAME(DEFINESOUND);
		GET_TAG_NAME(STARTSOUND);
		GET_TAG_NAME(STOPSOUND);
		GET_TAG_NAME(DEFINEBUTTONSOUND);
		GET_TAG_NAME(SOUNDSTREAMHEAD);
		GET_TAG_NAME(SOUNDSTREAMBLOCK);
		GET_TAG_NAME(DEFINELOSSLESS);
		GET_TAG_NAME(DEFINEBITSJPEG2);
		GET_TAG_NAME(DEFINESHAPE2);
		GET_TAG_NAME(DEFINEBUTTONCXFORM);
		GET_TAG_NAME(PROTECT);

		GET_TAG_NAME(PLACEOBJECT2);

		GET_TAG_NAME(REMOVEOBJECT2);

		GET_TAG_NAME(DEFINESHAPE3);
		GET_TAG_NAME(DEFINETEXT2);
		GET_TAG_NAME(DEFINEBUTTON2);
		GET_TAG_NAME(DEFINEBITSJPEG3);
		GET_TAG_NAME(DEFINELOSSLESS2);
		GET_TAG_NAME(DEFINEEDITTEXT);

		GET_TAG_NAME(DEFINESPRITE);

		GET_TAG_NAME(FRAMELABEL);

		GET_TAG_NAME(SOUNDSTREAMHEAD2);
		GET_TAG_NAME(DEFINEMORPHSHAPE);

		GET_TAG_NAME(DEFINEFONT2);

		GET_TAG_NAME(EXPORTASSETS);
		GET_TAG_NAME(IMPORTASSETS);
		GET_TAG_NAME(ENABLEDEBUGGER);
		GET_TAG_NAME(INITACTION);
		GET_TAG_NAME(DEFINEVIDEOSTREAM);
		GET_TAG_NAME(VIDEOFRAME);
		GET_TAG_NAME(DEFINEFONTINFO2);

		GET_TAG_NAME(ENABLEDEBUGGER2);
		GET_TAG_NAME(SCRIPTLIMITS);
		GET_TAG_NAME(SETTABINDEX);

		GET_TAG_NAME(FILEATTRIBUTES);
		GET_TAG_NAME(PLACEOBJECT3);
		GET_TAG_NAME(IMPORTASSETS2);

		GET_TAG_NAME(DEFINEALIGNZONES);
		GET_TAG_NAME(CSMTEXTSETTINGS);
		GET_TAG_NAME(DEFINEFONT3);
		GET_TAG_NAME(SYMBOLCLASS);
		GET_TAG_NAME(METADATA);
		GET_TAG_NAME(DEFINESCALINGGRID);

		GET_TAG_NAME(DOABCDEFINE);
		GET_TAG_NAME(DEFINESHAPE4);
		GET_TAG_NAME(DEFINEMORPHSHAPE2);

		GET_TAG_NAME(DEFINESCENEANDFRAMELABELDATA);
		GET_TAG_NAME(DEFINEBINARYDATA);
		GET_TAG_NAME(DEFINEFONTNAME);
		GET_TAG_NAME(STARTSOUND2);
	}

	return "unknown name";
}

const std::vector<ExecuteTag*>&	MovieDefImpl::getPlaylist(int frameNumber)
{
  return _playList[frameNumber];
}

// Movie* MovieDefImpl::createInstance()
// {
//   Movie* m = new Movie(this);

//   SpriteInstance* movie = new SpriteInstance(this, m);
//   m->setMovie(movie);
//   m->getMovie()->executeFrameTags(0);
	
//   return m;
//}

void MovieDefImpl::addExecuteTag(ExecuteTag* c)
{
  _playList[getLoadingFrame()].push_back(c);
}

BitmapCharacterDef*	MovieDefImpl::getBitmapCharacter(int character_id)
{
  std::map<int, BitmapCharacterDef*>::iterator itr = _bitmapCharacters.find(character_id);
  //ASSERT(itr != _bitmapCharacters.end());
  return itr->second;
}

void MovieDefImpl::addCharacter(int character_id, CharacterDef* c)
{
  _characters.insert(std::make_pair(character_id, c));
}

CharacterDef* MovieDefImpl::getCharacter(int chId)
{
	std::map<int, CharacterDef*>::iterator itr = _characters.find(chId);
	if(itr == _characters.end())
	{
		return NULL;
	}

	return itr->second;
}

void MovieDefImpl::addBitmapCharacter(int character_id, BitmapCharacterDef* ch)
{
  _bitmapCharacters.insert(std::make_pair(character_id, ch));
}

void MovieDefImpl::addBitmapInfo(BitmapInfo* bi)
{
  _bitmapList.push_back(bi);
}

void MovieDefImpl::setBackgroundColor(RGBA color)
{
  _sprite->setBGColor(color);
}
