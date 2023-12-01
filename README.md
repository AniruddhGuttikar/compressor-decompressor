
Compressor and Decompressor in C
Overview
This repository contains a simple command-line program written in C for compressing and decompressing text files. The compression algorithm utilized is based on heap data structures. The program includes two main components: compressor.c for compressing files and decompressor.c for decompressing previously compressed files.

Prerequisites
Ensure that you have any latest version of C installed on your system.

Usage
Place your input text file in the same directory as the program files and name it "input_file.txt".

Open a terminal or command prompt.

Compile the compressor and decompressor programs separately using the following commands:

bash
Copy code
gcc compressor.c -o compressor
gcc decompressor.c -o decompressor
Run the compressor on the input file:

bash
Copy code
./compressor
This will generate a compressed file named "compressed_file.bin" in the same directory.

To decompress the file, run the decompressor:

bash
Copy code
./decompressor
This will produce the decompressed file named "decompressed_file.txt" in the same directory.

File Structure
compressor.c: Source code for the compressor program.
decompressor.c: Source code for the decompressor program.
heap.h: Header file containing necessary data structures and functions for heap operations.
Contributing
Feel free to contribute to the project by opening issues or submitting pull requests.
