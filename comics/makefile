all: 	comics.html
	cd comics/dc    && ./combine.sh
	cd comics/conan && ./sortbysize.sh
	formatcomics comics/marvel/* 
	formatcomics comics/dc/* 
	formatcomics comics/conan/* 
	formatcomics comics/other/* 
	formatcomics comics/vertigo/* 
	../assemble comics  
clean:	comics.html
	rm -f *html
comics.html:
	touch comics.html


