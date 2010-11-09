PROG=demo
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic-errors -pipe -march=native -g

.PHONY: clean pack

all: $(PROG)

$(PROG): $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CXX) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	$(RM) *.o xlogin00.zip $(PROG)

pack:
	zip -r xlogin00.zip *.cpp *.h Makefile doc README*
