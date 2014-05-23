build/06.pdf: 06.tex
	lualatex --output-directory=build $^
	lualatex --output-directory=build $^

clean:
	rm -rf build/*

open: build/06.pdf
	open build/06.pdf
