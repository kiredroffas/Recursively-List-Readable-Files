.PHONY: clean  #Tell make that 'clean' is not associated with building files

#Compile source files and produce executable
all : readable.o
	cc -o readable readable.c
#Enter filepath to print all files/directories from : make run fp=____
run :
	./readable "$(fp)"
#Test on big file system, all fprintf errors to stderr (2) go to a created file error.txt
test :
	./readable / 2>error.txt
#Remove object and temp files
clean :
	rm readable *.o error.txt
