build/03.pdf: 04.tex
	lualatex --output-directory=build $^
	lualatex --output-directory=build $^

clean:
	rm -rf build/*

open: build/04.pdf
	open build/04.pdf
