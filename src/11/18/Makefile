CXX=clang++
CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++11
CXXFLAGS+=$(shell pkg-config --cflags eigen3)

all: schroedinger datafiles plotfiles schroedinger.mp4

schroedinger: schroedinger.cc

datafiles:
	mkdir -p data/
	./schroedinger

plotfiles:
	mkdir -p plots/
	./plot.py

schroedinger.mp4:
	avconv -r 10 -i plots/%04d.png -c:v libx264 $@

clean:
	rm -f schroedinger
	rm -f schroedinger.mp4
	rm -rf plots
	rm -rf data
