STD = -std=c99
WARN = -Wall
OPTI = -O3
INCL = -I dsfmt/
MTFLAGS = -msse2 -DDSFMT_MEXP=2203 -DHAVE_SSE2
#GTKFLAGS = `pkg-config --cflags --libs gtk+-3.0` -DGTK_DISABLE_DEPRECATED=1
CCFLAGS = $(OPTI) $(WARN) $(STD) $(INCL) $(MTFLAGS) #$(GTKFLAGS)
DEBUGFLAGS = -g

all:
	gcc ${CCFLAGS} dsfmt/dSFMT.c sim.c pcasim.c -o pcasim

debug:
	gcc ${CFLAGS} ${DEBUGFLAGS} dsfmt/dSFMT.c sim.c pcasim.c -o pcasim
