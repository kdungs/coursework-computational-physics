CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

pi: pi.cc

saw: saw.cc

data.txt: saw
	./saw

plots: data.txt
	./saw_RN.py

clean:
	rm -f pi
	rm -f saw
	rm -f *.pdf
	rm -f data.txt
