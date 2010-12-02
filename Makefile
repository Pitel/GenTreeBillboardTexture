PROG=demo
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra  -pedantic-errors  -pipe -march=native -g `sdl-config --cflags --libs` 

# -pedantic-errors - oddelana kvuli variable length array error
CXXFLAGS_GUI=-O2 -Wall -Wextra -pedantic -pipe -march=native -g `pkg-config --cflags --libs gtk+-2.0`
CXXFLAGS_SDL=-O2 -Wall -Wextra -pedantic -pipe -march=native -g `sdl-config --cflags --libs` `pkg-config SDL_image  --cflags  --libs`

LATEX=pdfcslatex

OBJS=GenTreeBillboardTexture.o grammar.o param.o rules.o vis.o

.PHONY: clean pack doc

all: $(PROG)

# Obecne pravidlo
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

#Zavislosti (g++ -MM *.cpp)
GenTreeBillboardTexture.o: GenTreeBillboardTexture.cpp GenTreeBillboardTexture.h param.h grammar.h rules.h vis.h
grammar.o: grammar.cpp grammar.h rules.h param.h
param.o: param.cpp param.h grammar.h rules.h
rules.o: rules.cpp rules.h
vis.o: vis.cpp vis.h grammar.h rules.h param.h

#Slinkovani
$(PROG): $(PROG).cpp $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(PROG).cpp -o $@

gui: demo_gui.cpp demo_sdl.cpp $(OBJS)
#	$(CXX) $(OBJS) $(CXXFLAGS_GUI) demo_gui.cpp -o demo_gui
	$(CXX) $(OBJS) $(CXXFLAGS_SDL) demo_sdl.cpp -o demo_sdl

doc:
	cd doc; $(LATEX) pgr.tex

clean:
	$(RM) *.o xlogin00.zip *.xpm $(PROG) $(PROG)_gui $(PROG)_sdl
	cd doc; $(RM) *.out *.log *.aux *.pdf

pack:
	zip -r xlogin00.zip *.cpp *.h Makefile doc README*
