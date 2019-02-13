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

  void setBGColor(RGBA color);
  void advance();
  void display();
protected:
  File _file;
  MovieDefImpl* _mdef;

  RGBA _bgColor;
  
  DisplayList _playList;
};
