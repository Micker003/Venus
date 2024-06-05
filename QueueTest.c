#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Define the coordinates struct
struct coordinates {
    int x;
    int y;
    int objectAtLocation;
};

// A structure to represent a queue
struct Queue {
    int front, rear, size;
    unsigned capacity;
    struct coordinates* array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (struct coordinates*)malloc(queue->capacity * sizeof(struct coordinates));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue. It changes rear and size
void enqueue(struct Queue* queue, struct coordinates item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("coordinates (%d, %d, %d) enqueued to queue\n", item.x, item.y, item.objectAtLocation);
}

// Function to remove an item from queue. It changes front and size
struct coordinates dequeue(struct Queue* queue)
{
    if (isEmpty(queue)) {
        struct coordinates empty = {INT_MIN, INT_MIN, INT_MIN};
        return empty;
    }
    struct coordinates item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
struct coordinates front(struct Queue* queue)
{
    if (isEmpty(queue)) {
        struct coordinates empty = {INT_MIN, INT_MIN, INT_MIN};
        return empty;
    }
    return queue->array[queue->front];
}

// Function to get rear of queue
struct coordinates rear(struct Queue* queue)
{
    if (isEmpty(queue)) {
        struct coordinates empty = {INT_MIN, INT_MIN, INT_MIN};
        return empty;
    }
    return queue->array[queue->rear];
}

// Driver program to test above functions
int main()
{
    struct Queue* queue = createQueue(1000);

    struct coordinates coord1 = {10, 20, 1};
    struct coordinates coord2 = {30, 40, 2};
    struct coordinates coord3 = {50, 60, 3};
    struct coordinates coord4 = {70, 80, 4};

    enqueue(queue, coord1);
    enqueue(queue, coord2);
    enqueue(queue, coord3);
    enqueue(queue, coord4);

    struct coordinates dequeued = dequeue(queue);
    printf("coordinates (%d, %d, %d) dequeued from queue\n\n", dequeued.x, dequeued.y, dequeued.objectAtLocation);

    struct coordinates frontItem = front(queue);
    printf("Front item is coordinates (%d, %d, %d)\n", frontItem.x, frontItem.y, frontItem.objectAtLocation);

    struct coordinates rearItem = rear(queue);
    printf("Rear item is coordinates (%d, %d, %d)\n", rearItem.x, rearItem.y, rearItem.objectAtLocation);

    return 0;
}
