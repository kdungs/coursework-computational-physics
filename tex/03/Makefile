build/03.pdf: 03.tex
	lualatex --output-directory=build $^
	lualatex --output-directory=build $^

clean:
	rm -rf build/*

open: build/03.pdf
	open build/03.pdf
