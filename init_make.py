#python3

import sys
import os
import re

#all src dirs
dirs = ". swf swf/base render"
temp_dir = "temp"
target = "dummy"
cpp_flags = os.popen("sdl2-config --cflags").read().replace("\n","")+" -Wall -g2 -ggdb -O0 -I. -Iswf -Iswf/base -Ideps/zlib -Ideps/jpeg -Ideps -Ideps/regex" + " -DHAVE_CONFIG_H -DPCRE_STATIC -DSUPPORT_UTF -DSUPPORT_UTF8 "

cpp_libs = os.popen("sdl2-config --libs").read().replace("\n","")+" -lfreetype -lSDL2_ttf -lSDL2_mixer -lSDL2_image"

zlib_src = """
deps/zlib/adler32.c
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
deps/zlib/zutil.c
"""

jpeg_src = """
deps/jpeg/jaricom.c
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
deps/jpeg/jmemansi.c
"""

pcre_src = """
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
deps/regex/pcre_xclass.c
"""

cpp_files = zlib_src.split() + jpeg_src.split() + pcre_src.split()
############################
#create tempdir
if not os.path.exists(temp_dir):
	os.makedirs(temp_dir)
	
#########################################
#split line
for one_dir in dirs.split():
	cpp_files = cpp_files + [cpp for cpp in os.listdir(one_dir) if cpp.endswith('.cpp') or cpp.endswith('.c') or cpp.endswith('.cc')]

get_temp_o = lambda cpp:temp_dir+"/"+cpp.replace(".cpp",".o").replace(".c",".o").replace(".cc",".o").replace("/",".")
	
objs = list(map(get_temp_o, cpp_files))

phony = """
.PHONY: clean all
all:$(TARGET)
""".replace("$(TARGET)", target)

flags = """
CPPFLAGS := $(cpp_flags)
CPPLIBS := $(cpp_libs)

""".replace("$(cpp_flags)", cpp_flags).replace("$(cpp_libs)", cpp_libs)

def get_gen_temp_o(cpp):
	cmd = "gcc -MM $(CPPFLAGS) "+cpp
	return os.popen(cmd.replace("$(CPPFLAGS)", cpp_flags)).read()+"\tgcc -c $< -o $@ $(CPPFLAGS)\n".replace("$(CPPFLAGS)", cpp_flags)
	
def write_temp_o(make_file, cpp_files):
	for cpp in cpp_files:		
		temp_o = get_temp_o(cpp)		
		make_file.write(re.sub(r'^.*\.o\s*:', temp_o+":", get_gen_temp_o(cpp)))
		make_file.write("\n")
		print(temp_o+":"+cpp)
	
target_cmd = """
$(TARGET):$(OBJS)
	gcc $(OBJS) -o $@ $(CPPLIBS)
""".replace("$(TARGET)", target).replace("$(OBJS)", " ".join(objs))

clean_cmd = """
clean:
	rm -f *.d; \
	rm -f *.o; \
	rm -f $(TARGET)
""".replace("$(TARGET)", target)

with open("Makefile", "w") as make_file:
	print("start write makefile")
	make_file.write(phony)
	print(phony)
	make_file.write(flags)
	print(flags)
	write_temp_o(make_file, cpp_files)
	make_file.write(target_cmd)
	print(target_cmd)
	make_file.write(clean_cmd)
	print(clean_cmd)
	print("end write makefile")
