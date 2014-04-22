CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

forest_percolation: forest_percolation.cpp

nice_plots: nice_plots.cpp

perculation: perculation.cc

plots: nice_plots
	mkdir -p ./plots ./data
	./nice_plots && python plot_forest.py

fit: forest_percolation
	mkdir -p ./plots ./data
	./forest_percolation && python fit_p.py

clean:
	rm -f perculation
	rm -f forest_percolation
	rm -rf ./plots
	rm -rf ./data
