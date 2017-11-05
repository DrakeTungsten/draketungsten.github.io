all :   linux.html
	../assemble linux  
	cp /backup/docs/programming/scripts/*sh .
	./sh2txt
	chmod -x *.txt
linux.html: 
	touch linux.html
clean:
	rm -f linux.html
	rm -f *.sh
	rm -f *.txt
