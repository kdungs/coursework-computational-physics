CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

pi_approximation: pi_approximation.cc

saw: saw.cc

RN.pdf: saw
	./saw | ./saw_RN.py

clean:
	rm -f pi_approximation
	rm -f saw
	rm -f *.pdf
