build/05.pdf: 05.tex
	lualatex --output-directory=build $^
	lualatex --output-directory=build $^

clean:
	rm -rf build/*

open: build/05.pdf
	open build/05.pdf
