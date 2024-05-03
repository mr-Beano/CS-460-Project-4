// CS 460 Operating Systems Project 4 - Page Replacement Simulation
// Dean Feller 4/23/2024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define USAGE "Usage: ./pagesim (FIFO|LRU) CACHESIZE INPUT\n"
#define MISSCOST 5
#define WRITECOST 10


/* Structure to represent a page in this simulation. */
struct Page {
    char operation;
    int number;
    struct Page *prev, *next;
};

/* Allocate a new Page, fill in its info, and return a pointer to it. */
struct Page* newPage(char operation, int pageNum) {
    struct Page *page = (struct Page*) malloc(sizeof(struct Page));
    if (page == NULL) {
        printf("Failed to allocate page");
        exit(EXIT_FAILURE);
    }
    page->operation = operation;
    page->number = pageNum;
    page->prev = NULL;
    page->next = NULL;
    return page;
}



/* Implementation of a queue of Pages as a doubly-linked list. */
struct Queue {
    int size;       // Amount of pages queue can hold
    int length;     // Amount of pages currently in the queue
    struct Page *head, *tail;
};

/*
Allocate a new Queue, initialize its values, and return a pointer to it.
When you're done with a Queue, be sure to free it with freeQueue() (not just free()!)
*/
struct Queue* newQueue(int size) {
    struct Queue *queue = (struct Queue*) malloc(sizeof(struct Queue));
    if (queue == NULL) {
        printf("Failed to allocate queue");
        exit(EXIT_FAILURE);
    }
    queue->size = size;
    queue->length = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

/* Free the given queue and all of its contents. */
void freeQueue(struct Queue *queue) {
    struct Page *current = queue->head;
    struct Page *next;

    // While there are still Processes in the queue, save the next one,
    // free this one, then move over to the next one
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(queue);
}


/* Appends page to the tail of the queue. */
void addLast(struct Queue *queue, struct Page *page) {
    if (page != NULL) {
        if (queue->length == 0) {
            queue->head = page;
            queue->tail = page;
        }
        else {
            queue->tail->next = page;
            page->prev = queue->tail;
            queue->tail = page;
        }
        queue->length++;
    }
}


/*
Removes the given page from the queue by reassigning the next/prev
pointers of its neighbors. In doing so, the next/prev pointers 
of the page are set to NULL. Returns the removed page, or NULL
if it doesn't exist.
*/
struct Page* removePage(struct Queue *queue, struct Page *page) {
    if (page != NULL) {
        if (page == queue->head) queue->head = page->next;
        if (page == queue->tail) queue->tail = page->prev;
        if (page->prev != NULL) page->prev->next = page->next;
        if (page->next != NULL) page->next->prev = page->prev;
        page->next = NULL;
        page->prev = NULL;
        queue->length--;
    }
    return page;
}

/*
Check to see if the page with the given number is in the queue.
Returns the matching page, or NULL if not found.
*/
struct Page* getPage(struct Queue *queue, int pageNum) {
    struct Page *current = queue->head;

    while (current != NULL) {
        if (current->number == pageNum) break;
        current = current->next;
    }

    return current;
}




// The four stats to output
int refNum = 0;         // Total number of page references
int missNum = 0;        // Total number of page misses
int missTime = 0;       // Total time units for page misses
int writeTime = 0;      // Total time units for page writes

enum Algorithm { FIFO, LRU };


/* Run the paging simulation with the given algorithm. */
void runSimulation(int alg, struct Queue *queue, char* inputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Failed to open file %s\n", inputFileName);
        exit(EXIT_FAILURE);
    }
    char* line = NULL;
    size_t len = 0;

    while(getline(&line, &len, inputFile) != -1) {
        char operation = *strtok(line, " \n");
        int pageNum = atoi(strtok(NULL, " \n"));

        // Check if page is in queue. If so, update its operation
        struct Page *page = getPage(queue, pageNum);
        if (page != NULL) {
            page->operation = page->operation | operation;  // If either one is W, it will be W

            // If using LRU, move page to end of queue
            if (alg == LRU) {
                removePage(queue, page);
                addLast(queue, page);
            }
        }

        else {
            // If the queue is full, remove the head, adding write cost if necessary
            if (queue->length == queue->size) {
                struct Page *removedPage = removePage(queue, queue->head);
                if (removedPage != NULL && removedPage->operation == 'W') writeTime += WRITECOST;
                free(removedPage);
            }
            
            // Append new page to end of queue, adding read cost
            addLast(queue, newPage(operation, pageNum));
            missTime += MISSCOST;
            missNum++;
        }

        refNum++;
    }

    free(line);
    fclose(inputFile);
}


int main(int argc, char* argv[]) {
    // If there aren't enough args, exit
    if (argc < 4) {
        printf(USAGE);
        exit(EXIT_FAILURE);
    }

    enum Algorithm alg = -1;

    if (strcmp(argv[1], "FIFO") == 0) alg = FIFO;
    else if (strcmp(argv[1], "LRU") == 0) alg = LRU; 

    int cacheSize = atoi(argv[2]);
    char* inputFileName = argv[3];

    struct Queue *pageFrames = newQueue(cacheSize);

    runSimulation(alg, pageFrames, inputFileName);

    printf("1. Total number of page references = %d\n", refNum);
    printf("2. Total number of page misses = %d\n", missNum);
    printf("3. Time units for page misses = %d\n", missTime);
    printf("4. Time units for page writes = %d\n", writeTime);

    freeQueue(pageFrames);
    return 0;
}