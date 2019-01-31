cmake_minimum_required (VERSION 3.5)

project(dummy)

aux_source_directory(swf DIR_SWF)
aux_source_directory(swf/base DIR_BASE)
set(DUMMYSRC main.cpp ${DIR_SWF} ${DIR_BASE})

SET(TOP ${PROJECT_SOURCE_DIR})

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(SDL2_INCLUDE_DIRS "-I/usr/include/SDL2 -D_REENTRANT")
set(SDL2_LIBRARIES "-L/usr/lib/x86_64-linux-gnu -lSDL2 -lfreetype -lSDL2_ttf -lSDL2_mixer -lSDL2_image")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  ${SDL2_INCLUDE_DIRS})

include_directories(swf swf/base deps/zlib deps/jpeg deps deps/regex)

ADD_DEFINITIONS(-DHAVE_CONFIG_H -DPCRE_STATIC -DSUPPORT_UTF -DSUPPORT_UTF8)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

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

  set(REGEXSRC
    deps/regex/pcrecpp.cc
    deps/regex/pcreposix.c
    deps/regex/pcre_byte_order.c
    deps/regex/pcre_chartables.c
    deps/regex/pcre_compile.c
    deps/regex/pcre_config.c
    deps/regex/pcre_dfa_exec.c
    deps/regex/pcre_exec.c
    deps/regex/pcre_fullinfo.c
    deps/regex/pcre_get.c
    deps/regex/pcre_globals.c
    deps/regex/pcre_jit_compile.c
    deps/regex/pcre_maketables.c
    deps/regex/pcre_newline.c
    deps/regex/pcre_ord2utf8.c
    deps/regex/pcre_refcount.c
    deps/regex/pcre_scanner.cc
    deps/regex/pcre_stringpiece.cc
    deps/regex/pcre_string_utils.c
    deps/regex/pcre_study.c
    deps/regex/pcre_tables.c
    deps/regex/pcre_ucd.c
    deps/regex/pcre_valid_utf8.c
    deps/regex/pcre_version.c
    deps/regex/pcre_xclass.c)
  
set(SRC ${ZLIBSRC} ${DUMMYSRC} ${JPEGSRC} ${REGEXSRC})

add_executable(dummy ${SRC})
target_link_libraries(dummy ${SDL2_LIBRARIES})
