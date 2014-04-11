CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

pi_approximation: pi_approximation.cc

saw: saw.cc

walk.pdf: saw
	./saw | ./plot_walk.py

clean:
	rm -f pi_approximation
	rm -f saw
	rm -f *.pdf
