#pragma once

#include "movie_definition_sub.h"

class File;
class Stream;
class ExecuteTag;
class BitmapCharacterDef;
class SWFPlayer;

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
		
  MovieDefImpl(SWFPlayer* player);
  bool readHead(File* f);
  void readTags();

  float	getFrameRate() const { return _frameRate; }
  float	getWidthPixels() const { return ceilf(TWIPS_TO_PIXELS(_frameSize.width())); }
  float	getHeightPixels() const { return ceilf(TWIPS_TO_PIXELS(_frameSize.height())); }

  virtual const std::vector<ExecuteTag*>&	getPlaylist(int frameNumber);
  //virtual Movie*	createInstance();
  virtual void		addExecuteTag(ExecuteTag* c);
  virtual BitmapCharacterDef*	getBitmapCharacter(int character_id);
  virtual void	addBitmapCharacter(int character_id, BitmapCharacterDef* ch);
  virtual void	addCharacter(int character_id, CharacterDef* c) { ASSERT(c); _characters.insert(std::make_pair(character_id, c)); }
  virtual void	addBitmapInfo(BitmapInfo* bi);
  virtual void  setBackgroundColor(RGBA color);
private:
  SWFPlayer* _player;
  int	_version;
  int _fileEndPos;

  int	_loadedLength;

  Stream*		_str;
  File*			_zlibFile;

  RECT		_frameSize;
  float		_frameRate;

  std::vector<BitmapInfo*>									_bitmapList;
  std::vector<std::vector<ExecuteTag*> >		_playList;
  std::vector<std::vector<ExecuteTag*> >		_initActionList;

  std::map<int, CharacterDef*>							_characters;
  std::map<int, BitmapCharacterDef*>				_bitmapCharacters;
};
