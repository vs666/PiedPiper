compress: huffman.o minHeap.o compress.o huffman.h minHeap.h
	gcc -o compress huffman.o minHeap.o compress.o

decompress: huffman.o codeSearchTree.o decompress.o huffman.h codeSearchTree.h
	gcc -o decompress huffman.o codeSearchTree.o decompress.o

huffman.o: huffman.c
	gcc -c huffman.c

minHeap.o: minHeap.c
	gcc -c minHeap.c

compress.o: compress.c
	gcc -c compress.c

decompress.o: decompress.c
	gcc -c decompress.c

codeSearchTree.o: codeSearchTree.c
	gcc -c codeSearchTree.c

clean:
	rm *.o compress decompress
