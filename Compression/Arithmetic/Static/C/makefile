compress : \
	filehandler.o \
	model.o	\
	compressor.c

	gcc -o compress $^

filehandler.o : filehandler.h filehandler.c 
	gcc -c -g filehandler.c

model.o : model.h model.c 
	gcc -c -g model.c

clean:
	rm -rf *.o 

