#pragma once

#include "GL/glew.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_draw.h>

//#include <GL/gl.h>
//#include <GL/glu.h>

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>

extern "C" {
#include "jpeglib.h"
}

#ifndef NULL
#define NULL 0
#endif



// Compile-time constant size of array.
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

// Compile-time assert.  Thanks to Jon Jagger
// (http://www.jaggersoft.com) for this trick.
#define COMPILER_ASSERT(x)	switch(0){case 0: case x:;}
#define ASSERT(expr) do{if(!(expr)){ std::printf("%s:%d,%s\n",__FILE__,__LINE__,#expr); exit(1);}}while(0);
#define UNUSED(x) (void) (x)

#define TWIPS_TO_PIXELS(x)	((x) / 20.f)
#define PIXELS_TO_TWIPS(x)	((x) * 20.f)

typedef char                SInt8;
typedef unsigned char       UInt8;
typedef unsigned char       Uint8;
typedef short               SInt16;
typedef short               Sint16;
typedef short               sint16;
typedef unsigned short      UInt16;
typedef unsigned short      Uint16;
typedef int									SInt32;
typedef int									Sint32;
typedef int									sint32;
typedef unsigned int				UInt32;
typedef unsigned int				Uint32;
typedef UInt8								Byte;
typedef UInt8								byte;
typedef float 							coord_component;
typedef int64_t 						Sint64;
typedef int64_t 						sint64;

#include "util.h"
#include "file.h"
#include "basic_types.h"
#include "def.h"
#include "zlibfile.h"
#include "container.h"
#include "membuf.h"

static inline void fatalError(const std::string& info)
{
	std::cout << info << std::endl;
	std::cout << "Enter Any Key To Exit" << std::endl;
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}
