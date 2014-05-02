CXXFLAGS=-O2 -Wall -Werror -pedantic -std=c++11

random_test: random_test.cpp Randinium.cpp

plots: random_test
	mkdir -p ./data ./plots
	./random_test && python plots.py

clean:
	rm -f ./random_test
	rm -rf ./data ./plots
