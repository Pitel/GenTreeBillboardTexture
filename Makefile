PROG=demo
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra  -pedantic-errors  -pipe -march=native -g
SDL=`sdl-config --cflags --libs`
GTK=`pkg-config --cflags --libs gtk+-2.0`

LATEX=pdfcslatex

OBJS=GenTreeBillboardTexture.o grammar.o param.o rules.o vis.o

.PHONY: clean pack doc

all: $(PROG)

# Obecne pravidlo
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL) -c $<

#Zavislosti (g++ -MM *.cpp)
GenTreeBillboardTexture.o: GenTreeBillboardTexture.cpp GenTreeBillboardTexture.h param.h grammar.h rules.h vis.h
grammar.o: grammar.cpp grammar.h rules.h param.h
param.o: param.cpp param.h grammar.h rules.h
rules.o: rules.cpp rules.h
vis.o: vis.cpp vis.h grammar.h rules.h param.h

#Slinkovani
$(PROG): $(PROG)_gui.cpp $(PROG)_sdl.cpp $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(GTK) $(SDL) $(PROG)_gui.cpp -o $(PROG)_gui
	$(CXX) $(OBJS) $(CXXFLAGS) $(SDL) $(PROG)_sdl.cpp -o $(PROG)_sdl

doc:
	cd doc; $(LATEX) pgr.tex

clean:
	$(RM) *.o xlogin00.zip *.xpm $(PROG) $(PROG)_gui $(PROG)_sdl
	cd doc; $(RM) *.out *.log *.aux *.pdf

pack:
	zip -r xlogin00.zip *.cpp *.h Makefile doc README*
