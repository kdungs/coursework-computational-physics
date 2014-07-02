CXX=clang++
CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++11
CXXFLAGS+=$(shell pkg-config --cflags eigen3)

schroedinger: schroedinger.cc

clean:
	rm -f schroedinger
