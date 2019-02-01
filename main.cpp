#include "stdafx.h"
#include "render/window.h"

int main(int argc, char** argv)
{
  Window w;
  w.init("test.swf");
  w.run();
  
  return 0;
}

