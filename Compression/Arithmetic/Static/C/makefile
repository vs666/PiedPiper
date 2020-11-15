executable : \
	filehandler.o \
	model.o	\
	decompressor.c

	gcc -o executable $^
	rm -rf *.o 

filehandler.o : filehandler.h filehandler.c 
	gcc -c -g filehandler.c

model.o : model.h model.c 
	gcc -c -g model.c
