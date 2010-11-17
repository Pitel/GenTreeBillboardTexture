PROG=demo
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra  -pedantic-errors  -pipe -march=native -g

# -pedantic-errors - oddelana kvuli variable length array error
CXXFLAGS_GUI=-O2 -Wall -Wextra -pipe -march=native -g `pkg-config --cflags --libs gtk+-2.0` 

.PHONY: clean pack

#all: $(PROG)


#$(PROG): $(patsubst %.cpp,%.o,$(wildcard *.cpp))
#	$(CXX) $^ $(LOADLIBES) $(LDLIBS) -o $@


all: demo
OBJS= GenTreeBillboardTexture.o grammar.o param.o rules.o vis.o


param.o: param.cpp param.h
	$(CXX) $(CXXFLAGS) -c param.cpp

rules.o: rules.cpp rules.h
	$(CXX) $(CXXFLAGS) -c rules.cpp

grammar.o: grammar.cpp grammar.h param.h rules.h
	$(CXX) $(CXXFLAGS) -c grammar.cpp

vis.o: vis.cpp vis.h grammar.h
	$(CXX) $(CXXFLAGS) -c vis.cpp

GenTreeBillboardTexture.o: GenTreeBillboardTexture.cpp GenTreeBillboardTexture.h grammar.h param.h rules.h
	$(CXX) $(CXXFLAGS) -c GenTreeBillboardTexture.cpp

demo:  $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) demo.cpp -o demo

gui:  $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS_GUI) demo_gui.cpp -o demo_gui





##### PHONY:


clean:
	$(RM) *.o xlogin00.zip *.xpm $(PROG) $(PROG)_gui

pack:
	zip -r xlogin00.zip *.cpp *.h Makefile doc README*
