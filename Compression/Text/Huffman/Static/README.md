# Huffman Static Compression for Text Files

* Compresses text files to a little more than half the size
* To use:
	* First build from source
	```
	$ make compress decompress
	```
	* Then for compression, run:
	```
	$ ./compress <input_path> <output_path>
	```
	* And for decompression, run:
	```
	$ ./decompress <input_path> <output_path>
	```
	* The same information can also be found by running the binaries with -h or --help flag:
	```
	$ ./compress --help
	$ ./decompress --help
	```
* TODO
	1. Add comments in the code.
	2. Reduce the time using buffers
