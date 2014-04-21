CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

perculation_test: perculation_test.cpp

perculation: perculation.cc

plots: perculation_test
	mkdir ./plots ./data
	./perculation_test && python plot_forest.py

clean:
	rm -f perculation
	rm -f perculation_test
	rm -rf ./plots
	rm -rf ./data
