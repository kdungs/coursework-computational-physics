CXX=clang++
CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++11
CXXFLAGS+=$(shell pkg-config --cflags eigen3)

lennard-jones: lennard-jones.cc

clean:
	rm -f lennard-jones
