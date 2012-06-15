STD = -std=c99
WARN = -Wall
OPTI = -O3
INCL = -I dsfmt/
MTFLAGS = -msse2 -DDSFMT_MEXP=2203 -DHAVE_SSE2
LIBFLAGS = -lreadline -lpng
#GTKFLAGS = `pkg-config --cflags --libs gtk+-3.0` -DGTK_DISABLE_DEPRECATED=1
CCFLAGS = $(OPTI) $(WARN) $(STD) $(INCL) $(MTFLAGS) $(LIBFLAGS) #$(GTKFLAGS)
DEBUGFLAGS = -g -O0

all: pcasim test

pcasim: xalloc.c sim.c diff.c utils.c internals.c commands.c pgmlib.c pcasim.c pgmlib.c diff.c
	gcc ${CCFLAGS} dsfmt/dSFMT.c xalloc.c sim.c utils.c internals.c commands.c pgmlib.c diff.c pcasim.c -o pcasim

debug: xalloc.c sim.c diff.c utils.c internals.c commands.c pgmlib.c pcasim.c pgmlib.c
	gcc ${CCFLAGS} ${DEBUGFLAGS} dsfmt/dSFMT.c xalloc.c sim.c utils.c internals.c commands.c pgmlib.c diff.c pcasim.c -o pcasim

test: xalloc.c sim.c diff.c utils.c internals.c commands.c test.c pgmlib.c
	gcc ${CCFLAGS} ${DEBUGFLAGS} dsfmt/dSFMT.c xalloc.c sim.c utils.c internals.c commands.c pgmlib.c diff.c test.c -o test

clean:
	rm -f test pcasim *.hi *.o PECA PECAPOLY
