#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "embeddedImplementationInterface.c"
#include "AlgorithmTestCases.c"
#include "BFStest.h"
#define initialx 0
#define initialy 0
#define MAX_SIZE 100


int IRthreshold = 100;  //threshold for which IR sensor data is determined to be color black

//create a struct called coordinates which can maintain the location of the robot
struct coordinates {
    int x; 
    int y;
    int objectAtLocation;
};

// Struct for ArrayList structure
typedef struct {
    struct coordinates *array;
    size_t size;
    size_t capacity;
} ArrayList;

ArrayList list;         //initialization of arraylist which will store all of the coordinates as well as their types

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
    printf("ENQUEUEING STARTED\n");
    if (isFull(q)) {
        fprintf(stderr, "Queue is full\n");
       return;
    }
    
    if (isEmpty(q)) {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->data[q->rear] = value;
    printf("Enqueued: (%d, %d)\n", value.x, value.y);
}

/**
 * method to dequeue an element from the queue
*/
struct coordinates dequeue(Queue* q) {
    if (isEmpty(q)) {
        fprintf(stderr, "BALLS\n");
        exit(1);
    }
    printf("dequeueing STARTED");
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
    //blockType can be 4 for "small" || 5 for "big" || 6 for null
    int blockType; 
    //block color can be any string corresponding to the RBG color returned by embedded software method the integer represents the rgb value
    int blockColor; 
    //parity value, 1 is true and 0 is false
    int cliffPresent;
    //parity value for hole
    int holePresent;
    //parity value for boundary 
    int boundaryPresent; 

};









/**
 * moveRobot method controls the moving of the robot.
 * 0 -> forward, 1 -> Right, 2 -> left, 3 -> backwards
*/
struct coordinates moveRobot(struct coordinates inputCoordinate, int dirSelVal) {
    //dirSelVal determines the direction that the robot needs to move in 

    struct coordinates c;

    switch (dirSelVal) {
        case 0: //move the robot one step forward
            c.x = inputCoordinate.x;
            c.y = inputCoordinate.y + 1;
            c.objectAtLocation = inputCoordinate.objectAtLocation;
            moveRobotForwardOrBackward(1); 
            break;
        case 1: //move the robot one step to the right
            c.x = inputCoordinate.x + 1;
            c.y = inputCoordinate.y;
            c.objectAtLocation = inputCoordinate.objectAtLocation;
            rotateRobot(0);
            moveRobotForwardOrBackward(1);
            break;
        case 2: //move the robot one step to the left
            c.x = inputCoordinate.x - 1;
            c.y = inputCoordinate.y;
            c.objectAtLocation = inputCoordinate.objectAtLocation;
            rotateRobot(1);
            moveRobotForwardOrBackward(1);
            break;
           
        case 3: //move the robot one step backwards
            c.x = inputCoordinate.x;
            c.y = inputCoordinate.y - 1;
            c.objectAtLocation = inputCoordinate.objectAtLocation;
            moveRobotForwardOrBackward(0);
            break;
    
    }
    return c;
}



/**
 * returns if square is free and has been discovered
*/
int findObjectAtCoordinate(ArrayList *list, struct coordinates target) {
    int objectAtCoordinate;
    for (size_t i = 0; i < list->size; i++) {
        if (list->array[i].x == target.x && list->array[i].y == target.y) {
            objectAtCoordinate = list->array->objectAtLocation;
        } else {
            return 0;
        }
    }
    
    if (objectAtCoordinate = 0) {
        return 1; 
    } else {
        return 0;
    }
}

// Prototypes for the functions
void avoidCollisions(int axisOfMovement, struct coordinates current, struct coordinates target, int xDistanceToCover, int yDistanceToCover);
void robotNavigation(struct coordinates current, struct coordinates destination);

/**
 * method to enable robot to avoid collisions with objects on the map 
 * axis of movement indicates whether the robot is moving on the x axis or the y axis, x -> 0, y -> 1
*/
void avoidCollisions(int axisOfMovement, struct coordinates current, struct coordinates target, int xDistanceToCover, int yDistanceToCover) {
    //ensure that when the robot is moving on the x axis that it always returns to the coordinate that it started on 
    switch (axisOfMovement)
    {
    case 0:
        
        //collision avoidance when robot is moving on x axis
        struct coordinates upCoor = current;
        upCoor.y = current.y + 1;
        struct coordinates downCoor = current;
        downCoor.y = current.y - 1;
        int upCheck = findObjectAtCoordinate(&list, upCoor);
        int downCheck = findObjectAtCoordinate(&list, downCoor);

        if (upCheck = 1) {
            current = moveRobot(current, 0);
            robotNavigation(current, target);
        } else if (downCheck = 1) {
            current = moveRobot(current, 3);
            robotNavigation(current,target);
        } else {
            //if both sides are blocked then we have recursive call after backtracking by one
            if (xDistanceToCover > 0) {
                current = moveRobot(current, 2);
            } else if (xDistanceToCover < 0) {
                current = moveRobot(current, 1);
            } 
            int xDistanceToCover = target.x - current.x;
            avoidCollisions(0, current, target, xDistanceToCover, yDistanceToCover);
        }

        break;
    case 1:
        //collision avoidance when robot is moving on y axis
            struct coordinates rightCoor = current;
            rightCoor.x = current.x + 1;
            struct coordinates leftCoor = current;
            leftCoor.x = current.x - 1;
            int rightCheck = findObjectAtCoordinate(&list, rightCoor);
            int leftCheck = findObjectAtCoordinate(&list, leftCoor);

            if (rightCheck = 1) {
                current = moveRobot(current, 1);
                robotNavigation(current, target);
            } else if (leftCheck = 1) {
                current = moveRobot(current, 2);
                robotNavigation(current,target);
            } else {
                //if both sides are blocked then we have recursive call after backtracking by one
                if (yDistanceToCover > 0) {
                    current = moveRobot(current, 3);
                } else if (yDistanceToCover < 0) {
                    current = moveRobot(current, 0);
                }
                int yDistanceToCover = target.y - current.y;
                avoidCollisions(1, current, target, xDistanceToCover, yDistanceToCover);
            }
        break;

    }
}


// Prototype for the checkSquare function
void robotNavigation(struct coordinates current, struct coordinates destination);

/**
 * method that creates a path for the robot to navigate to the 
 * coordinate that is selected from the queue
*/
void robotNavigation(struct coordinates current, struct coordinates destination) {
    //currentidea: BFS algorithm ensures that the coordinates below are always explored
    //forming a pyramid of sorts, so it is always possible to navigate down and to the right or left.
    int yDistanceToCover = destination.y - current.y;
    int xDistanceToCover = destination.x - current.x;


    if (yDistanceToCover > 0 ) {
        while (current.y != destination.y) {
            struct coordinates upCoor = current; 
            upCoor.y = current.y + 1;
            int upCheck = findObjectAtCoordinate(&list, upCoor);
            switch (upCheck)
            {
            case 0:
                avoidCollisions(1, current, destination, xDistanceToCover, yDistanceToCover);  //if a collision is detected then avoid collision method is called
                break;
            case 1:
                current = moveRobot(current, 0);  //if the coordinate is empty then we move the robot up. 
                break;    
            default:
                break;
            }
        }
    } else if (yDistanceToCover < 0) {
        while (current.y != destination.y) {
            struct coordinates downCoor = current;
            downCoor.y = current.y - 1;
            int downCheck = findObjectAtCoordinate(&list, downCoor);
            switch (downCheck)
            {
            case 0:
                avoidCollisions(1, current, destination, xDistanceToCover, yDistanceToCover);
                break;
            case 1:
                current = moveRobot(current, 3); //if the coordinate is empty move the robot down
                break;
            default:
                break;
            }
        }
    } 

    if (xDistanceToCover > 0) {
        while (current.x != destination.x) {
            struct coordinates rightCoor = current; 
            rightCoor.x = current.x + 1;
            int rightCheck = findObjectAtCoordinate(&list, rightCoor);
            switch (rightCheck)
            {
            case 0:
                avoidCollisions(0,current,destination, xDistanceToCover, yDistanceToCover);
                break;
            case 1:
                current = moveRobot(current, 1);
                break;
            }
        }
    } else if (xDistanceToCover < 0) {
        while (current.x != destination.x) {
            struct coordinates leftCoor = current; 
            leftCoor.x = current.x - 1;
            int leftCheck = findObjectAtCoordinate(&list, leftCoor);
            switch (leftCheck)
            {
            case 0:
                avoidCollisions(0,current, destination, xDistanceToCover, yDistanceToCover);
                break;
            case 1:
                current = moveRobot(current, 2);
                break;
            }
        }
    }
    
   
    //TODO:make sure that the robot moves to the right or left to reach the target x coordinate without hitting an object. 
    //TODO: refactor the code in order to reduce complexity of robotNavigation method. 
    
}


/**
 * Method to convert RGB values to color integer
 * @param color Struct containing RGB values
 * @return Color integer based on the specified scheme
 */
int convertToColor(struct color color) {
    // Check for the presence of a dominant color
    if (color.R > 150 && color.R > color.G && color.R > color.B) {
        return 1; //1 represents red
    } else if (color.G > 150 && color.G > color.R && color.G > color.B) {
        return 2; //2 represents green
    } else if (color.B > 150 && color.B > color.R && color.B > color.G) {
        return 3; //3 represents blue
    } else if (color.R > 200 && color.G > 200 && color.B > 200) {
        return 4; //4 represents white
    } else {
        return 5; // 5 represents black
    }
}


// Prototype for the checkSquare function
struct squareType checkSquare(void);

/**
 * method to mesure the paramenters of the square infront of square currently occupied by robot 
*/
struct squareType checkSquare() {
    printf("square check started\n");
    struct squareType s; 
    struct IRSensors IR;

    int cliffDistance;
    cliffDistance = fwdDistanceData();
    //distance of each move is assumed to be 5 cm 
    if (cliffDistance < 50) {
        s.cliffPresent = 1;
    } else {
        s.cliffPresent = 0;
    }
    
    IR.sensor1Val = measureIRData().sensor1Val;
    IR.sensor2Val = measureIRData().sensor2Val;
    IR.sensor3Val = measureIRData().sensor3Val;
    IR.sensor4Val = measureIRData().sensor4Val;

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

    int blockHeight = dwdDistanceData();
    //height of smallest block assumed to be 3 cm and height of sensor above ground assumed as 6 cm
    if (blockHeight < 20) {              //checking for big block
        struct color colorstr = colorSensor();
        int color = convertToColor(colorstr);
        s.blockColor = color; 
        s.blockType = 5;
    } if (blockHeight < 30 && blockHeight > 20) {
        struct color colorstr = colorSensor();
        int color = convertToColor(colorstr);
        s.blockColor = color; 
        s.blockType = 4;
    } else {
        s.blockColor = 0; //0 represents no color 
        s.blockType = 6; //6 represents no block
    }


    return s; 

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

/**
 * translates the struct squareType into a simple character that can be stored in coordinates
*/
int returnSquareProperty(struct squareType s) {
    //TODO convert integer and character value inside of struct s to one of five possible char values and return said value. 
    if(s.boundaryPresent = 1) {
        return 1; //1 represents boundary
    } else if (s.cliffPresent = 1) {
        return 2; // 2 represents cliff
    } else if (s.holePresent = 1) {
        return 3; // 3 represents hole
    } else if (s.blockType != 6) {
        return 10*s.blockType + s.blockColor;
    } else {
        return 0; //0 represents empty
    }
}

/**
 * method to explore the square ahead of the currently occupied square
 * adds the coordinate explored to the array
 * cc represents the current coordinates and be represents the coordinate being explored
*/
int exploreForward(struct coordinates cc, struct Queue q) {
    printf("forward exploration started\n");

    int check = 0;
    struct coordinates be = cc;
    be.y = cc.y + 1;
    int duplication_check = isInArrayList(&list, be); 
    if(duplication_check > 0) { 
        check = 1; 
        return check; 
    }
    struct squareType st = checkSquare();
    int propertyAtCoordinate;
    propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate; // add the value for the object at location to the coordinate before adding to list 
    
    if (propertyAtCoordinate = 0) { //0 represents empty
        enqueue(&q, be);    //coordinate is only enqueued for further exploration if it is empty.
    }
    addElement(&list, be);
    return check;
}

/**
 * method to explore the square to the right of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreRight(struct coordinates cc, struct Queue q) {
    printf("RIGHT EXPLORATION STARTED\n");
    int check = 0;
    struct coordinates be = cc;
    be.x = cc.x + 1;
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
        return check;
    }
    rotateRobot(0);
    struct squareType st = checkSquare();
    int propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    
    if (propertyAtCoordinate = 0) {
        enqueue(&q, be);
    }
    addElement(&list, be);
    rotateRobot(1);
    return check;
}

/**
 * method to explore the square to the left of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreLeft(struct coordinates cc, struct Queue q) {
    printf("LEFT EXPLORATION STARTED\n");
    int check = 0;
    struct coordinates be = cc;
    be.x = cc.x - 1; 
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
        return check;
    }
    rotateRobot(1);
    struct squareType st = checkSquare();
    int propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    if (propertyAtCoordinate = 0) {
        enqueue(&q, be);
    }
    addElement(&list, be);
    rotateRobot(0);
    return check;
}

/**
 * method to explore the square behind of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreBehind(struct coordinates cc, struct Queue q) {
    printf("BACKWARD EXPLORATION STARTED\n");
    int check = 0;
    struct coordinates be = cc;
    be.y = cc.y - 1;
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
        return check;
    }
    rotateRobot(2);
    struct squareType st = checkSquare();
    int propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    
    if (propertyAtCoordinate = 0) {
        enqueue(&q, be);
    }
    addElement(&list, be);
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


//prototype for BFS method
void BFS(struct coordinates currentCoordinate);

/**
* primary control method calling other mehtods in program 
* @pre nil
* @post 
* @return int   
*/
int main(void) {
    struct coordinates currentCoordinate;
    //create the initial coordinate which can be assumed as empty
    currentCoordinate.x = 0;
    currentCoordinate.y = 0;

    currentCoordinate.objectAtLocation = 0;

    initArrayList(&list);
    addElement(&list, currentCoordinate);


    // Verify that currentCoordinate has been added to the ArrayList
    for (size_t i = 0; i < list.size; ++i) {
        printf("ArrayList Element %zu - x: %d, y: %d, objectAtLocation: %d\n", 
               i, list.array[i].x, list.array[i].y, list.array[i].objectAtLocation);
    }

    BFS(currentCoordinate);

    //send the arraylist to the MQTT server for visualization
    //TODO
    //After sending the data this code terminates
    return 0;
}

//prototype for test method print Queue
void printQueue(Queue* q);

 /**
 * BFS search algorithm to search to explore the entire grid
*/
void BFS(struct coordinates currentCoordinate) {
    int duplication_check = 0;
    struct coordinates beingExplored = currentCoordinate; 
    struct coordinates navigateTo;
    Queue q;            //create a BFS queue
    initQueue(&q);      //initialize the BFS queue using the initQueue() method
    printf("BFS STARTED\n");

    //while loop which runs until a duplicate is found or until the time limit has been reached
    while (duplication_check < 1) {

        //explore all of the squares adjacent to currently occupied square
        duplication_check = exploreForward(beingExplored, q);
        duplication_check = duplication_check + exploreRight(beingExplored, q);
        duplication_check = duplication_check + exploreLeft(beingExplored, q);
        duplication_check = duplication_check + exploreBehind(beingExplored, q); //if duplication_check > 0 break while loop
        
        //printQueue(&q);

        navigateTo = dequeue(&q);        //deque one of the visited coordinates from the queue and 
        //add it to a new struct called coordinates
        robotNavigation(beingExplored, navigateTo);
        beingExplored = navigateTo; //once the robotNavigation() method has been executed set the robot's beingExplored coordinate to the navigateTo coordinate. 
      
        
    }

    printf("Duplicate has been found\n");
    main();
}


/**
 * Method to print all of the elements in a queue at any moment ini time 
*/
void printQueue(Queue* q) {
    int i;
    if (isEmpty(q)) {
        printf("NOTHING HAS BEEN ENQUEUED FUCK\n");
        return;
    }
    printf("Queue elements: ");
    i = q->front;
    if (q->front <= q->rear) {
        while (i <= q->rear) {
            printf("(%d,%d) ", q->data[i].x, q->data[i].y);
            i++;
        }
    } else {
        while (i <= MAX_SIZE - 1) {
            printf("(%d,%d) ", q->data[i].x, q->data[i].y);
            i++;
        }
        i = 0;
        while (i <= q->rear) {
            printf("(%d,%d) ", q->data[i].x, q->data[i].y);
            i++;
        }
    }
    printf("\n");
}

