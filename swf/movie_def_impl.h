#pragma once

#include "movie_definition_sub.h"

class File;
class Stream;
class ExecuteTag;
class BitmapCharacterDef;
class SpriteInstance;

class MovieDefImpl:public MovieDefinitionSub
{
public:
  // Unique id of a gameswf resource
  enum { _classId = AS_MOVIE_DEF };
  virtual bool is(int classId) const
  {
    if (_classId == classId) return true;
    else return MovieDefinitionSub::is(classId);
  }
		
  MovieDefImpl(SpriteInstance* sprite);
  bool readHead(File* f);
  void readTags();
	static char* getTagName(int tag);
	
  float	getFrameRate() const { return _frameRate; }
  float	getWidthPixels() const { return ceilf(TWIPS_TO_PIXELS(_frameSize.width())); }
  float	getHeightPixels() const { return ceilf(TWIPS_TO_PIXELS(_frameSize.height())); }
  int   getVersion() { return _version; }
  
  virtual const std::vector<ExecuteTag*>&	getPlaylist(int frameNumber);
  //virtual Movie*	createInstance();
  virtual void		addExecuteTag(ExecuteTag* c);
  virtual BitmapCharacterDef*	getBitmapCharacter(int character_id);
  virtual void	addBitmapCharacter(int character_id, BitmapCharacterDef* ch);
  virtual void	addCharacter(int character_id, CharacterDef* c);
	CharacterDef* getCharacter(int chId);
	
  virtual void	addBitmapInfo(BitmapInfo* bi);
  virtual void  setBackgroundColor(RGBA color);
private:
  SpriteInstance* _sprite;
  int _version;
  int _fileEndPos;

  int _loadedLength;

  Stream* _str;
  File*	_zlibFile;

  RECT _frameSize;
  float _frameRate;

  std::vector<BitmapInfo*> _bitmapList;
  std::vector<std::vector<ExecuteTag*> > _playList;
  std::vector<std::vector<ExecuteTag*> > _initActionList;

  std::map<int, CharacterDef*> _characters;
  std::map<int, BitmapCharacterDef*> _bitmapCharacters;
};
