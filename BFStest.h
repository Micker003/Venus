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

// Struct for ArrayList structure
typedef struct {
    struct coordinates *array;
    size_t size;
    size_t capacity;
} ArrayList;

struct Queue* createQueue(unsigned capacity);
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, struct coordinates item);
struct coordinates dequeue(struct Queue* queue);
struct coordinates front(struct Queue* queue);
struct coordinates rear(struct Queue* queue);



#endif // BFSTEST_H
