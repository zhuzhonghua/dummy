cmake_minimum_required (VERSION 3.5)

project(dummyplayer)

aux_source_directory(swf DIR_SWF)
aux_source_directory(base DIR_BASE)
aux_source_directory(render DIR_RENDER)
set(DUMMYSRC src/main.cpp ${DIR_SWF} ${DIR_BASE} ${DIR_RENDER})

SET(TOP ${PROJECT_SOURCE_DIR})

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(. deps/include deps/include/SDL2 deps/include/SDL2_Image deps/include/SDL2_mixer deps/include/SDL2_ttf)
include_directories(gfx swf deps/zlib deps/jpeg deps render base)

set(SDL2_INCLUDE_DIRS "${TOP}/deps/include/SDL2")
set(SDL2_LIBRARIES "-L${TOP}/deps/libs  -lSDL2 -lfreetype -lSDL2_ttf -lSDL2_mixer -lSDL2_image")

set(DUMMY_FLAGS "-lm -liconv -lGLEW \
	-Wl,-framework,ForceFeedback \
	-lobjc -Wl,-framework,CoreVideo \
	-Wl,-framework,Cocoa \
	-Wl,-framework,Carbon \
	-Wl,-framework,IOKit \
	-Wl,-framework,CoreAudio \
	-Wl,-framework,AudioToolbox \
	-Wl,-framework,AudioUnit \
	-Wl,-framework,OpenGL")

ADD_DEFINITIONS(-DHAVE_CONFIG_H -DPCRE_STATIC -DSUPPORT_UTF -DSUPPORT_UTF8)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  ${DUMMY_FLAGS})

set(ZLIBSRC deps/zlib/adler32.c
    deps/zlib/compress.c
    deps/zlib/crc32.c
    deps/zlib/deflate.c
    deps/zlib/gzio.c
    deps/zlib/infback.c
    deps/zlib/inffast.c
    deps/zlib/inflate.c
    deps/zlib/inftrees.c
    deps/zlib/trees.c
    deps/zlib/uncompr.c
    deps/zlib/zutil.c)

set(JPEGSRC deps/jpeg/jaricom.c
						deps/jpeg/jcapimin.c
						deps/jpeg/jcapistd.c
						deps/jpeg/jcarith.c
						deps/jpeg/jccoefct.c
						deps/jpeg/jccolor.c
        		deps/jpeg/jcdctmgr.c
						deps/jpeg/jchuff.c
						deps/jpeg/jcinit.c
						deps/jpeg/jcmainct.c
						deps/jpeg/jcmarker.c
						deps/jpeg/jcmaster.c
        		deps/jpeg/jcomapi.c
						deps/jpeg/jcparam.c
						deps/jpeg/jcprepct.c
						deps/jpeg/jcsample.c
						deps/jpeg/jctrans.c
						deps/jpeg/jdapimin.c
        		deps/jpeg/jdapistd.c
						deps/jpeg/jdarith.c
						deps/jpeg/jdatadst.c
						deps/jpeg/jdatasrc.c
						deps/jpeg/jdcoefct.c
						deps/jpeg/jdcolor.c
        		deps/jpeg/jddctmgr.c
						deps/jpeg/jdhuff.c
						deps/jpeg/jdinput.c
						deps/jpeg/jdmainct.c
						deps/jpeg/jdmarker.c
						deps/jpeg/jdmaster.c
        		deps/jpeg/jdmerge.c
						deps/jpeg/jdpostct.c
						deps/jpeg/jdsample.c
						deps/jpeg/jdtrans.c
						deps/jpeg/jerror.c
						deps/jpeg/jfdctflt.c
        		deps/jpeg/jfdctfst.c
						deps/jpeg/jfdctint.c
						deps/jpeg/jidctflt.c
						deps/jpeg/jidctfst.c
						deps/jpeg/jidctint.c
						deps/jpeg/jquant1.c
        		deps/jpeg/jquant2.c
						deps/jpeg/jutils.c
						deps/jpeg/jmemmgr.c
						deps/jpeg/jmemansi.c)

set(REGEXSRC regex/pcrecpp.cc
						 regex/pcreposix.c
						 regex/pcre_byte_order.c
						 regex/pcre_chartables.c
						 regex/pcre_compile.c
						 regex/pcre_config.c
						 regex/pcre_dfa_exec.c
						 regex/pcre_exec.c
						 regex/pcre_fullinfo.c
						 regex/pcre_get.c
						 regex/pcre_globals.c
						 regex/pcre_jit_compile.c
						 regex/pcre_maketables.c
						 regex/pcre_newline.c
						 regex/pcre_ord2utf8.c
						 regex/pcre_refcount.c
						 regex/pcre_scanner.cc
						 regex/pcre_stringpiece.cc
						 regex/pcre_string_utils.c
						 regex/pcre_study.c
						 regex/pcre_tables.c
						 regex/pcre_ucd.c
						 regex/pcre_valid_utf8.c
						 regex/pcre_version.c
						 regex/pcre_xclass.c)

set(SRC ${ZLIBSRC} ${DUMMYSRC} ${JPEGSRC} ${REGEXSRC})

add_executable(dummyplayer ${SRC})
target_link_libraries(dummyplayer ${SDL2_LIBRARIES})
