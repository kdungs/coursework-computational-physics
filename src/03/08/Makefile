CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

all: plots

distributions: distributions.cc

plots: distributions
	./distributions
	./hist.py

clean:
	rm -f distributions
	rm -f *.txt
	rm -f *.pdf
