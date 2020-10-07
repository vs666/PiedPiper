encrypt: \
				prg.o \
				util.o \
				encryptor.h \
				encryptor.c \
	
	gcc -o encrypt $^
	rm -f *.o

util.o: util.h 
	gcc -g -c util.c

prg.o: prg.h util.o
	gcc -g -c prg.c 
