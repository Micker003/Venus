#ifndef BFSTEST_H
#define BFSTEST_H

#define MAX_SIZE 100

struct coordinates {
    int x;
    int y;
    int objectAtLocation;
};

typedef struct {
    struct coordinates data[MAX_SIZE];
    int front;
    int rear;
} Queue;

void initQueue(Queue* q);
int isEmpty(Queue* q);
int isFull(Queue* q);
void enqueue(Queue* q, struct coordinates value);
struct coordinates dequeue(Queue* q);
void printQueue(Queue* q);

#endif // BFSTEST_H
