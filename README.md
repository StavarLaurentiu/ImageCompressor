This program is a C implementation of image compression using a quadtree.

The program takes an uncompressed PPM image file as input and compresses it using a quadtree. The compressed image is saved in a binary file, along with the compression tree used. Additionally, the program can decompress the compressed image and recreate the original uncompressed image.

• Usage:

The program takes three input arguments:
    -> command (c1, c2, or d)
    -> the input file name
    -> the output file name

The "c1" command compresses the image using a quadtree and writes the number of levels in the compression tree, the number of nodes in the compression tree, and the side length of the largest square remaining uncompressed to a text file.
The "c2" command compresses the image using a quadtree and saves the compression tree to a binary file.
The "d" command decompresses the image using the compression tree and saves the decompressed image to a PPM file.

• Compilation:
    -> make
    -> make clean

• Execution:
    ./quadtree -c1 in.ppm out.txt
    ./quadtree -c2 in.ppm out.bin
    ./quadtree -d in.bin out.ppm

• Error message:
    Usage ./quadtree [-c1 factor | -c2 factor | -d] [input_file] [output_file]

• Data structures used:
    -> Quadtree
    -> Pixel (containing the color of a pixel with three integer values)
    -> Queue implemented with a singly linked list
    -> Singly linked list
    -> Matrix
    -> Vectors
