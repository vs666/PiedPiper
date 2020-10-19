compress: huffman.o minHeap.o compress.o
	gcc -o compress $^

huffman.o: huffman.c
	gcc -c huffman.c

minHeap.o: minHeap.c
	gcc -c minHeap.c

compress.o: compress.c
	gcc -c compress.c

clean:
	rm *.o compress
