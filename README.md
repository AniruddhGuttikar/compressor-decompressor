# Compressor and Decompressor in C

## Overview

This repository contains a simple command-line program written in C for compressing and decompressing text files. The compression algorithm utilized is based on heap data structures. The program includes two main components: `compressor.c` for compressing files and `decompressor.c` for decompressing previously compressed files.

## Prerequisites

Ensure that you have any latest version of C installed on your system.

## Usage

1. Place your input text file in the same directory as the program files and name it "input_file.txt".
2. Open a terminal or command prompt.
3. Compile the compressor and decompressor programs separately using the following commands:

    ```bash
    gcc compressor.c -o compressor
    gcc decompressor.c -o decompressor
    ```

4. Run the compressor on the input file:

    ```bash
    ./compressor
    ```

    This will generate a compressed file named "output_file.bin" in the same directory.

5. To decompress the file, run the decompressor:

    ```bash
    ./decompressor
    ```

    This will produce the decompressed file named "file-decompressed.txt" in the same directory.

## File Structure

- `compressor.c`: Source code for the compressor program.
- `decompressor.c`: Source code for the decompressor program.
- `heap.h`: Header file containing necessary data structures and functions for heap operations.

## Contributing

Feel free to contribute to the project by opening issues or submitting pull requests. Feel free to
help me get rid of some horrible memory leaks.

Happy compressing and decompressing! 🎉
