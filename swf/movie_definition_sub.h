#pragma once

#include "movie_definition.h"
class ExecuteTag;

class MovieDefinitionSub:public MovieDefinition
{
public:
  // Unique id of a gameswf resource
  enum { _classId = AS_MOVIE_DEF_SUB };
  virtual bool is(int classId) const
  {
    if (_classId == classId) return true;
    else return MovieDefinition::is(classId);
  }

  MovieDefinitionSub():_frameCount(0),
		       _loadingFrame(0)
  {
  }
	
  int	getLoadingFrame() const { return _loadingFrame; }
  void	incLoadingFrame()	{ _loadingFrame++;	}
  virtual int	getFrameCount() { return _frameCount; }
  void	setFrameCount(int frames)
  {
    _frameCount = frames;

    // ALEX: some SWF files have been seen that have 0-frame sprites.
    // The Macromedia player behaves as if they have 1 frame.
    if (_frameCount < 1)
    {
      _frameCount = 1;
    }
  }

  virtual int   getVersion() = 0;
  virtual const std::vector<ExecuteTag*>&	getPlaylist(int frameNumber) = 0;
  virtual void	addExecuteTag(ExecuteTag* c) = 0;
  virtual BitmapCharacterDef*	getBitmapCharacter(int character_id) = 0;
  virtual void	addBitmapCharacter(int character_id, BitmapCharacterDef* ch) = 0;
  virtual void	addCharacter(int id, CharacterDef* ch) = 0;
  virtual void	addBitmapInfo(BitmapInfo* ch) = 0;
  virtual void  setBackgroundColor(RGBA color) = 0;
protected:
  int	_frameCount;
  int	_loadingFrame;
};
