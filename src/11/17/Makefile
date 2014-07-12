CXX=clang++
CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++11
CXXFLAGS+=$(shell pkg-config --cflags eigen3)

lennard-jones: lennard-jones.cc

everything: lennard-jones
	mkdir -p ./data
	./lennard-jones
	python pro_animation.py

clean:
	rm -f lennard-jones
	rm -rf ./data
	rm -rf ./plots
	rm -f ./*.mp4
	rm -f energies.txt
