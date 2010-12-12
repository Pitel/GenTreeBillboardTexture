##################################################
# nastaveni
CFLAGS_COMMON=-pipe -Wall -ggdb
CC=gcc
CXX=g++
PROG=billboard

##################################################
# nastaveni knihoven s OpenGL
ifeq ($(OS), Windows_NT)
	CFLAGS=$(CFLAGS_COMMON) -DWIN32 -I. -IGL -Wl,--enable-auto-import
	LIBS=-lopengl32 -lglu32 -lSDL
else
	CFLAGS=$(CFLAGS_COMMON) `sdl-config --cflags` -I. -IGL -L/usr/lib
	LIBS=`sdl-config --libs`  -L/usr/lib -lGL -lGLU -lXmu -lX11 -lXi -L/usr/X11R6/lib
endif

CXXFLAGS=$(CFLAGS)

##################################################
# obecne cile

3D=$(PROG)

DEPS=pgr.o GLee.o
DFILES=../grammar.o ../param.o ../rules.o ../vis.o ../GenTreeBillboardTexture.o

.PHONY: all clean

all: $(3D)

clean:
	$(RM)  GLee.o pgr *.o $(PROG)
	$(RM) *~

##################################################
# pravidla

#%.o: %.c
#	$(CC) -c -o $@ $^ $(CFLAGS)

#%.o: %.cpp
#	$(CXX) -c -o $@ $^ $(CFLAGS)

$(PROG): $(PROG).cpp $(DFILES)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS) $(DEPS)

##################################################
# zavislosti

GLee.o: GLee.c

pgr.o: pgr.cpp

pgr.cpp: pgr.h

$(PROG).cpp: $(DEPS)
