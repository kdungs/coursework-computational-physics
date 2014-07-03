CXX=clang++
CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++11
CXXFLAGS+=$(shell pkg-config --cflags eigen3)

schroedinger: schroedinger.cc

datafiles: schroedinger
	mkdir -p data/
	./schroedinger

plotfiles: datafiles
	mkdir -p plots/
	./plot.py

out.mp4: plotfiles
	ffmpeg -r 10 -pattern_type glob -i 'plots/psi_*.png' -c:v libx264 -r 30 out.mp4

clean:
	rm -f schroedinger
	rm -f out.mp4
	rm -rf plots
	rm -rf data
