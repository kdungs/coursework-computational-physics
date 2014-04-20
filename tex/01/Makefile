build/01.pdf: 01.tex
	lualatex --output-directory=build $^
	lualatex --output-directory=build $^

clean:
	rm -rf build/*

open: build/01.pdf
	open build/01.pdf
