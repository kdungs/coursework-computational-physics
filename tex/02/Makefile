build/02.pdf: 02.tex
	lualatex --output-directory=build $^
	lualatex --output-directory=build $^

clean:
	rm -rf build/*

open: build/02.pdf
	open build/02.pdf
