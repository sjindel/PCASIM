STD = -std=c99
WARN = -Wall
OPTI = -O3
INCL = -I dsfmt/
MTFLAGS = -msse2 -DDSFMT_MEXP=2203 -DHAVE_SSE2
GTKFLAGS = `pkg-config --cflags --libs gtk+-3.0` -DGTK_DISABLE_DEPRECATED=1
CCFLAGS = $(OPTI) $(WARN) $(STD) $(INCL) $(MTFLAGS) $(GTKFLAGS)

all:
	gcc ${CCFLAGS} ./dsfmt/dSFMT.c sim.c driver.c -o sim
	gcc ${CCFLAGS} ./dsfmt/dSFMT.c test.c -o test
