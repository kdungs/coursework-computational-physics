CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

percolation_test: percolation_test.cpp

percolation: main.cc percolation.cc


plots: percolation_test
	mkdir ./plots ./data
	./percolation_test && python plot_forest.py

clean:
	rm -f percolation
	rm -f percolation_test
	rm -rf ./plots
	rm -rf ./data
	rm -f data.txt
	rm -f *.pdf
