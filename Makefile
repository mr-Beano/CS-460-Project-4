# CS 460 Project 4
# Dean Feller 4/29/2024

all:
	gcc -Wall -Werror pagesim.c -o pagesim -g

fifo: all
	./pagesim FIFO 10 pageref.txt

lru: all
	./pagesim LRU 10 pageref.txt


run: fifo lru


valfifo: all
	valgrind --leak-check=full --track-origins=yes ./pagesim FIFO 10 pageref.txt

vallru: all
	valgrind --leak-check=full --track-origins=yes ./pagesim LRU 10 pageref.txt

val: valfifo vallru


clean:
	rm -f pagesim