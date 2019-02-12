#pragma once

#include "display_list.h"

class MovieDefImpl;

class SWFPlayer{
public:
  SWFPlayer(const std::string& f);
  ~SWFPlayer();

  void readHead();
  void readTags();
  //void loadFile(const std::string& file);

  int getWidth();
  int getHeight();
  float getFrameRate();
  
  void advance();
  void display();
protected:
  File _file;
  MovieDefImpl* _mdef;

  DisplayList _playList;
};
