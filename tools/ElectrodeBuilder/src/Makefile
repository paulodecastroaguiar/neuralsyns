# makefile for NeuralSyns Stimulus Builder
# Mafalda Sousa 12-08-2010

CXX = gcc

PROGRAM = ElectrodeBuilder
BINDIR = ~/NeuralSyns/bin/
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c -lgsl -lgslcblas -Wall -Wno-unused-but-set-variable `wx-config --cxxflags` `pkg-config --cflags --libs gtk+-2.0`  -o $@ $<

all:	$(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) `wx-config --libs` -lgsl -lgslcblas -Wall -Wno-unused-but-set-variable

clean:
	rm -f *.o *.*~

install: ElectrodeBuilder
	mv ElectrodeBuilder $(BINDIR)
