
#include <stdio.h>
#include "BFStest.h"

int main() {
    Queue q;
    initQueue(&q);

    struct coordinates start = {0, 0, 0};
    enqueue(&q, start);

    struct coordinates coord1 = {1, 1, 0};
    struct coordinates coord2 = {2, 2, 0};
    struct coordinates coord3 = {3, 3, 0};

    enqueue(&q, coord1);
    enqueue(&q, coord2);
    enqueue(&q, coord3);

    printQueue(&q); // Check the state after enqueuing

    struct coordinates dequeued = dequeue(&q);
    printf("Dequeued: (%d, %d)\n", dequeued.x, dequeued.y);

    printQueue(&q); // Check the state after dequeuing

    return 0;
}
