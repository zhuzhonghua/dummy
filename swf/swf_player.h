#pragma once

class SpriteInstance;

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
  SpriteInstance* _sprite;
};
