all: 	comics.html
	#cd comics/dc    && ./combine.sh
	#cd comics/conan && ./sortbysize.sh
	formatcomics comics/marvel/* 
	formatcomics comics/dc/* 
	formatcomics comics/other/* 
	formatcomics comics/vertigo/* 
	# Dec 2021 - not sure what I was doing months ago
	# but I know it was new/something different for the Conan books
	# so to be safe, I am commenting out this next line
	# and I started editing the Conan html pages all by hand
	# the conan/sortbysize.sh was already commented out,
	# but since now I started editing all by hand, that's
	# probbaly the way to go now, unless I want to reverse-engineer
	# and or/remember whatever custom thing I was doing for Conan
	#formatcomics comics/conan/* 
	../assemble comics  
clean:	comics.html
	rm -f *html
comics.html:
	touch comics.html


