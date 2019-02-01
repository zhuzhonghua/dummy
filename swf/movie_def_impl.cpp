#include "stdafx.h"

#include "movie_def_impl.h"

#include "tag_loader.h"
//#include "movie.h"
//#include "sprite_instance.h"

MovieDefImpl::MovieDefImpl()
{
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

  std::printf("read header %d %d %d version %d\n", header[0], header[1], header[2], _version);
  std::printf("framecount %d\n", _frameCount);

  //readTags();

  return true;
}

void MovieDefImpl::readTags()
{
  while (_str->getPosition() < _fileEndPos)
  {
    TagInfo info;
    _str->openTag(&info);

    std::printf("read tag %d\n", info.tagType);
    if (info.tagType == Tag::SHOWFRAME)
    {
      incLoadingFrame();
    }
    else if (info.tagType == Tag::FILEATTRIBUTES)
    {
      printf("ignore fileattributes\n");
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

void MovieDefImpl::addBitmapCharacter(int character_id, BitmapCharacterDef* ch)
{
  _bitmapCharacters.insert(std::make_pair(character_id, ch));
}

void MovieDefImpl::addBitmapInfo(BitmapInfo* bi)
{
  _bitmapList.push_back(bi);
}
