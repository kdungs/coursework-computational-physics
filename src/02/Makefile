CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

forest_percolation: forest_percolation.cpp

forest_clustersize: forest_clustersize.cpp

nice_plots: nice_plots.cpp

perculation: perculation.cc

plots: nice_plots
	mkdir -p ./plots/nice ./data/plots
	./nice_plots && python plot_forest.py

fit: forest_percolation forest_clustersize
	mkdir -p ./plots/beta ./data/fit ./data/clustersize
	./forest_percolation && python fit_p.py
	./forest_clustersize && python fit_beta.py

clean:
	rm -f perculation
	rm -f forest_percolation
	rm -f nice_plots
	rm -rf ./plots
	rm -rf ./data
