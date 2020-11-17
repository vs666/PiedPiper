client : \
	server \
	util.o \
	prg.o \
	client.h \
	log.h \
	client.c 
	
	gcc -o client $^

server : \
	util.o \
	prg.o \
	server.h \
	log.h \
	server.c 

	gcc -o server $^

util.o : util.h util.c
	gcc -c -g util.c

prg.o :	prg.h prg.c util.c util.h
	gcc -c -g prg.c 

# clean : 
# 	rm -rf *.o client server





