all: decrypt.o encrypt.o expandKeys.o standardDefinitions.o
	make encrypt decrypt

encrypt: encrypt.o expandKeys.o standardDefinitions.o
	gcc -o encrypt encrypt.o expandKeys.o standardDefinitions.o

decrypt: decrypt.o expandKeys.o standardDefinitions.o
	gcc -o decrypt decrypt.o expandKeys.o standardDefinitions.o

encrypt.o: encrypt.c
	gcc -c encrypt.c

decrypt.o: decrypt.c
	gcc -c decrypt.c

expandKeys.o: expandKeys.c
	gcc -c expandKeys.c

standardDefinitions.o: standardDefinitions.c
	gcc -c standardDefinitions.c

clean:
	rm decrypt.o encrypt.o expandKeys.o standardDefinitions.o encrypt decrypt
