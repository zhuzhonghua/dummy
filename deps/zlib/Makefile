objects = adler32.o \
    compress.o \
    crc32.o \
    deflate.o \
    gzio.o \
    infback.o \
    inffast.o \
    inflate.o \
    inftrees.o \
    trees.o \
    uncompr.o \
    zutil.o

all:$(objects)
	ar -r libzlib.a $<

$(objects): %.o: %.c
	gcc -c $< -o $@