CC = gcc
STD = -std=c99
WARN = -Wall
OPTI = -O3
INCL = -I dsfmt/
MTFLAGS = -msse2 -DDSFMT_MEXP=2203 -DHAVE_SSE2
CCFLAGS = $(OPTI) $(WARN) $(STD) $(INCL) $(MTFLAGS)

all:
	gcc ${CCFLAGS} ./dsfmt/dSFMT.c sim.c -o sim
