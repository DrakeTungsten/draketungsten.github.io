all: 	comics.html formatcomics
	#cd comics/dc    && ./combine.sh
	#cd comics/conan && ./sortbysize.sh
	./formatcomics comics/marvel/* 
	./formatcomics comics/dc/* 
	./formatcomics comics/other/* 
	./formatcomics comics/vertigo/* 
	# all Conan edits are done manually now
	#formatcomics deprecated/comics/conan/* 
	../assemble comics  
clean:	comics.html
	rm -f *html
comics.html:
	touch comics.html
formatcomics:	formatcomics.cpp
	g++ -g -o formatcomics formatcomics.cpp	


