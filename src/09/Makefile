CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++11
CXXFLAGS+=$(shell pkg-config --cflags eigen3)

lanczos: lanczos.cc

clean:
	rm -f lanczos
