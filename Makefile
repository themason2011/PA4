CXX=g++

CXXFLAGS= -std=c++11 -Wall -g

BINARIES=snl

all: snl

snl: snl.cpp
	g++ -o snl snl.cpp

clean:
	/bin/rm -f *.o ${BINARIES}