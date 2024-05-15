#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "embeddedInitialImp.c"
#define initialx 0
#define initialy 0
#define MAX_SIZE 100

int IRthreshold = 100;  //threshold for which IR sensor data is determined to be color black
ArrayList list;         //initialization of arraylist which will store all of the coordinates as well as their types

//create a struct called coordinates which can maintain the location of the robot
struct coordinates {
    int x; 
    int y;
    char objectAtLocation;
};

// Struct for ArrayList structure
typedef struct {
    struct coordinates *array;
    size_t size;
    size_t capacity;
} ArrayList;


// Define the struct for the queue
typedef struct Queue {
    struct coordinates data[MAX_SIZE];
    int front; 
    int rear;
} Queue;

/**
 * method to initialize the bfs queue
*/
void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

/**
 * method to check if queue is empty
*/
bool isEmpty(Queue* q) {
    return (q->front == -1);
}

/**
 * method to check if the queue is full
*/
bool isFull(Queue* q) {
    return ((q->rear + 1) % MAX_SIZE == q->front);
}

/**
 * method to enqueue an element into the queue
*/
void enqueue(Queue* q, struct coordinates value) {
    if (isFull(q)) {
        fprintf(stderr, "Queue is full\n");
        exit(1);
    }
    
    if (isEmpty(q)) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->data[q->rear] = value;
}

/**
 * method to dequeue an element from the queue
*/
struct coordinates dequeue(Queue* q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue is empty\n");
        exit(1);
    }
    struct coordinates value = q->data[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_SIZE;
    }
    return value;
}

/**
 * method to get the front element in the queue
*/
struct coordinates front(Queue* q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue is empty\n");
        exit(1);
    }
    return q->data[q->front];
}

/**
 * method to get the rear element of the queue
*/
struct coordinates rear(Queue* q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue is empty\n");
        exit(1);
    }
    return q->data[q->rear];
}

//struct for the properties of a particular square as interpreted by sensor data
struct squareType {
    //blockType can be "small" || "big"
    char blockType; 
    //block color can be any string corresponding to the RBG color returned by embedded software method
    char blockColor; 
    //parity value, 1 is true and 0 is false
    int cliffPresent;
    //parity value for hole
    int holePresent;
    //parity value for boundary 
    int boundaryPresent; 

};




/**
* primary control method calling other mehtods in program 
* @pre nil
* @post 
* @return void   
*/
void main() {
    struct coordinates currentCoordinate;
    //create the initial coordinate which can be assumed as empty
    currentCoordinate.x = 0;
    currentCoordinate.y = 0;
    currentCoordinate.objectAtLocation = "Empty";
    
    initArrayList(&list);
    addElement(&list, currentCoordinate);
    BFS(currentCoordinate);

    //send the arraylist to the MQTT server for visualization
    //TODO
    //After sending the data this code terminates
    
}




/**
 * moveRobot method controls the moves of the robot 
*/
struct coordinates moveRobot(struct coordinates) {
    /**
     * assign integer dirSelVal a random integer between 0 and 3 
     * 0 moves the robot up, 1 moves the robot right, 2 moves 
     * the robot down and 3 moves the robot left. 
    */

    struct coordinates c1;

    int dirSelVal = rand() % 4;
    switch (dirSelVal) {
        case 0:
            struct coordinates cMove; 
            cMove.x = 0;
            cMove.y = 1;
            movedCoordinate(cMove, c1);
            break;
        case 1:
            cMove.x = 1;
            cMove.y = 0;
            movedCoordinate(cMove, c1);
            break;
        case 2:
            cMove.x = 0;
            cMove.y = -1;
            movedCoordinate(cMove, c1);
            break;
        case 3:
            cMove.x = -1;
            cMove.y = 0;
            movedCoordinate(cMove, c1);
            break;
    
    }
}

//irrelevant and needs to probably be removed. 
struct coordinates movedCoordinate(struct coordinates cMove, struct coordinates cInitial) {
    cInitial.x = cInitial.x + cMove.x;
    cInitial.y = cInitial.y + cMove.y;
}

/**
 * BFS search algorithm to search to explore the entire grid
*/
void BFS(struct coordinates currentCoordinate) {
    int duplication_check = 0;
    struct coordinates beingExplored; 
    Queue q;            //create a BFS queue
    initQueue(&q);      //initialize the BFS queue using the initQueue() method
    //while loop which runs until a duplicate is found or until the time limit has been reached
    while (duplication_check < 1) {
        //TODO (BFS ALGORITHM)
        //explore all of the squares adjacent to currently occupied square
        duplication_check = exploreForward(currentCoordinate, q);
        duplication_check = duplication_check + exploreRight(currentCoordinate, q);
        duplication_check = duplication_check + exploreLeft(currentCoordinate, q);
        duplication_check = duplication_check + exploreBehind(currentCoordinate, q); //if duplication_check > 0 break while loop
        

        //TODO: write the code to check for empty squares identified and to move to that square
        struct coordinates navigateTo = dequeue(&q);        //deque one of the visited coordinates from the queue and 
        //add it to a new struct called coordinates
        robotNavigation(currentCoordinate, navigateTo);
        currentCoordinate = navigateTo; //once the robotNavigation() method has been executed set the robot's current coordinate to the navigateTo coordinate. 
        //TODO: write the BFS algorithm namely implement the queue to store the level to know which squares need to be explord.
        
    }
    main();
}

/**
 * method that creates a path for the robot to navigate to the 
 * coordinate that is selected from teh queue
*/
void robotNavigation(struct coordinates current, struct coordinates destination) {

}

/**
 * method to explore the square ahead of the currently occupied square
 * adds the coordinate explored to the array
 * cc represents the current coordinates and be represents the coordinate being explored
*/
int exploreForward(struct coordinates cc, struct Queue q) {
    int check = 0;
    struct coordinates be;
    be.x = cc.x;
    be.y = cc.y + 1;
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
    }
    struct squareType st = checkSquare();
    char propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    addElement(&list, be);
    enqueue(&q, be);
    return check;
}

/**
 * method to explore the square to the right of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreRight(struct coordinates cc, struct Queue q) {
    int check = 0;
    struct coordinates be;
    be.x = cc.x + 1;
    be.y = cc.y;
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
    }
    rotateRobot(0);
    struct squareType st = checkSquare();
    char propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    addElement(&list, be);
    enqueue(&q, be);
    rotateRobot(1);
    return check;
}

/**
 * method to explore the square to the left of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreLeft(struct coordinates cc, struct Queue q) {
    int check = 0;
    struct coordinates be;
    be.x = cc.x - 1;
    be.y = cc.y;
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
    }
    rotateRobot(1);
    struct squareType st = checkSquare();
    char propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    addElement(&list, be);
    enqueue(&q, be);
    rotateRobot(0);
    return check;
}

/**
 * method to explore the square behind of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreBehind(struct coordinates cc, struct Queue q) {
    int check = 0;
    struct coordinates be;
    be.x = cc.x;
    be.y = cc.y - 1;
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
    }
    rotateRobot(2);
    struct squareType st = checkSquare();
    char propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    addElement(&list, be);
    enqueue(&q, be);
    rotateRobot(2);
    return check;
}


/**
 * method to initialize the arraylist 
*/
void initArrayList(ArrayList *list) {
    list->size = 0;
    list->capacity = 10; // Initial capacity, can be adjusted as needed
    list->array = (struct coordinates *)malloc(list->capacity * sizeof(struct coordinates));
    if (list->array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

/**
 * method to add an element to the arraylist 
*/
void addElement(ArrayList *list, struct coordinates element) {
    if (list->size >= list->capacity) {
        list->capacity *= 2; // Double the capacity if needed
        list->array = (struct coordinates *)realloc(list->array, list->capacity * sizeof(struct coordinates));
        if (list->array == NULL) {
            fprintf(stderr, "Memory reallocation failed\n"); //for testing
            exit(1);
        }
    }
    list->array[list->size++] = element;
}

// Cleanup function to free memory in the arraylist
void cleanupArrayList(ArrayList *list) {
    free(list->array);
}

//checks if a given coordinate is inside arrayList
int isInArrayList(ArrayList *list, struct coordinates target) {
    for (size_t i = 0; i < list->size; i++) {
        if (list->array[i].x == target.x && list->array[i].y == target.y) {
            return 1; 
        }
    }
    return 0;
}

/**
 * translates the struct squareType into a simple character that can be stored in coordinates
*/
char returnSquareProperty(struct squareType s) {
    //TODO convert integer and character value inside of struct s to one of five possible char values and return said value. 
}


/**
 * method to mesure the paramenters of the square infront of square currently occupied by robot 
*/
struct squareType checkSquare() {

    struct squareType s; 
    struct IRSensors IR;

    int cliffDistance = forwardDistanceData();
    //distance of each move is assumed to be 5 cm 
    if (cliffDistance < 5) {
        s.cliffPresent = 1;
    } else {
        s.cliffPresent = 0;
    }
    
    IR = measureIRData();
    if (IR.sensor1Val > IRthreshold && IR.sensor2Val > IRthreshold && IR.sensor3Val > IRthreshold) {
        s.boundaryPresent = 1; //if all three sensors indicate the surface is black we return true for boundary present
        s.holePresent = 0;
    } else if (IR.sensor1Val > IRthreshold || IR.sensor2Val > IRthreshold || IR.sensor3Val > IRthreshold) {
        s.boundaryPresent = 0; 
        s.holePresent = 1;
    } else {
        s.boundaryPresent = 0;
        s.holePresent = 0;
    }

    int blockHeight = downwardDistanceData();
    //height of smallest block assumed to be 3 cm and height of sensor above ground assumed as 6 cm
    if (blockHeight < 2) {              //checking for big block
        char color = colorSensor();
        s.blockColor = color; 
        s.blockType = "big";
    } if (blockHeight < 3 && blockHeight > 2) {
        char color = colorSendor();
        s.blockColor = color; 
        s.blockType = "small";
    } else {
        s.blockColor = NULL;
        s.blockType = NULL;
    }


    return s; 

}


 