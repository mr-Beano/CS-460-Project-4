# CS 460 Operating Systems Project 4 - Page Replacement Simulation
Dean Feller 4/23/2024

## Pagesim
Simulates the time required to replace pages from memory to free frames for new pages using different algorithms to measure the performance of those algorithms. The algorithms being tested are First In First Out (FIFO) and Least Recently Used (LRU).

## Build Instructions
In the Makefile are build and clean commands.

- `make all` or `make` - Compiles the executable `pagesim`

- `make clean` - Removes the executable

## Run Instructions

### Manually
After compiling, use this command to run the executable with the chosen algorithm and input file:

    ./pagesim (FIFO | LRU) CACHESIZE INPUT

For example, to run the FIFO algorithm with a cache size of 20 on the input file `pageref.text`:

    ./pagesim FIFO 20 pageref.txt

To run the LRU algorithm with a cache size of 50 on the input file `pageref.txt`:

    ./pagesim LRU 50 pageref.txt

NOTE: The order of parameters matters! Algorithm must come first, then number of pages, then input file.

### Makefile
The Makefile also has several commands for running the different algorithms separately, together, and with Valgrind. All of the run commands will compile the code before running, so doing `make all` beforehand is unnecessary when using them. All of these commands assume the existence of the input file `pageref.txt`. Input files with other names will be ignored, so if other input files are desired it will need to be run manually. Likewise, they use a page frame amount of 10, so any other amount will need to be run manually.

- `make fifo` - Runs the executable with the FIFO algorithm

- `make lru` - Runs the executable with the LRU algorithm

- `make run` - Runs both algorithms in order

- `make valfifo` - Runs the executable with the FIFO algorithm through Valgrind

- `make vallru` - Runs the executable with the LRU algorithm through Valgrind

- `make val` - Runs both algorithms in order through Valgrind