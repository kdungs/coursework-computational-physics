CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

logistic_map: logistic_map.cc

data/: logistic_map
	mkdir -p data
	./logistic_map

bifurcation_diagram.pdf: data/
	python bifurcation_diagram.py

open: bifurcation_diagram.pdf
	open bifurcation_diagram.pdf

clean:
	rm -f logistic_map
	rm -f bifurcation_diagram.pdf
	rm -rf data/
