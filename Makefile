PROG=demo
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic -pipe -march=native

.PHONY: clean

all: $(PROG)

$(PROG): $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CXX) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	$(RM) *.o $(PROG)
