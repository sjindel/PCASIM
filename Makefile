STD = -std=c99
WARN = -Wall
OPTI = -O3
INCL = -I dsfmt/
MTFLAGS = -msse2 -DDSFMT_MEXP=2203 -DHAVE_SSE2
#GTKFLAGS = `pkg-config --cflags --libs gtk+-3.0` -DGTK_DISABLE_DEPRECATED=1
CCFLAGS = $(OPTI) $(WARN) $(STD) $(INCL) $(MTFLAGS) #$(GTKFLAGS)
DEBUGFLAGS = -g -O0

all: pcasim test

pcasim: dsfmt/dSFMT.c sim.c utils.c internals.c pcasim.c
	gcc ${CCFLAGS} dsfmt/dSFMT.c sim.c utils.c internals.c pcasim.c -o pcasim

debug: dsfmt/dSFMT.c sim.c utils.c internals.c pcasim.c
	gcc ${CCFLAGS} ${DEBUGFLAGS} dsfmt/dSFMT.c sim.c utils.c internals.c pcasim.c -o pcasim

test: dsfmt/dSFMT.c sim.c utils.c internals.c test.c
	gcc ${CCFLAGS} ${DEBUGFLAGS} dsfmt/dSFMT.c sim.c utils.c internals.c test.c -o test

clean:
	rm test pcasim
