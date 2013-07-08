# makefile for NeuralSyns Network Builder
# Mafalda Sousa 11-03-2009

CXX = gcc

PROGRAM = NetBuilder
FLAGS = -Wall -Wno-unused-but-set-variable
BINDIR = ~/NeuralSyns/bin/
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c -lgsl -lgslcblas $(FLAGS) `wx-config --cxxflags` `pkg-config --cflags --libs gtk+-2.0`  -o $@ $<

all:	$(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) `wx-config --libs` -lgsl -lgslcblas $(FLAGS)

clean:
	rm -f *.o *.*~

install: NetBuilder
	mv NetBuilder $(BINDIR)
