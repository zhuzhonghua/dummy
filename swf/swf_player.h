#pragma once

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
protected:
  File _file;
  MovieDefImpl* _mdef;
};
