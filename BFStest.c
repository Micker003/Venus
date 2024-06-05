#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "embeddedImplementationInterface.c"
#include "AlgorithmTestCases.c"
//#include "BFStest.h"
#define initialx 0
#define initialy 0
#define MAX_SIZE 100


int IRthreshold = 260;  //threshold for which IR sensor data is determined to be color black

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
    int objectAtCoordinate = 0;
    for (size_t i = 0; i < list->size; i++) {
        if (list->array[i].x == target.x && list->array[i].y == target.y) {
            objectAtCoordinate = list->array->objectAtLocation;
        } else {
            objectAtCoordinate = 0;
        }
    }
    
    if (objectAtCoordinate == 0) {
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

        if (upCheck == 1) {
            current = moveRobot(current, 0);
            robotNavigation(current, target);
        } else if (downCheck == 1) {
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

            if (rightCheck == 1) {
                //printf("Is this the infinite print issue?");
                //exit(1);
                current = moveRobot(current, 1);
                robotNavigation(current, target);
            } else if (leftCheck == 1) {
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

    printf("current: (%d, %d, %d), destination: (%d, %d, %d)\n", current.x, current.y, current.objectAtLocation, destination.x, destination.y, destination.objectAtLocation );
 
    
    if (yDistanceToCover > 0 ) {
        printf("Covering Y distance up \n");
        while (current.y != destination.y) {
            struct coordinates upCoor = current; 
            upCoor.y = current.y + 1;
            int upCheck = findObjectAtCoordinate(&list, upCoor);
            switch (upCheck)
            {
            case 0:
                printf("avoiding up collision \n");
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
        printf("Covering Y distance down \n");
        while (current.y != destination.y) {
            struct coordinates downCoor = current;
            downCoor.y = current.y - 1;
            int downCheck = findObjectAtCoordinate(&list, downCoor);
            switch (downCheck)
            {
            case 0:
                printf("avoiding down collision \n");
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
        printf("Covering x distance right \n");
        while (current.x != destination.x) {
            struct coordinates rightCoor = current; 
            rightCoor.x = current.x + 1;
            int rightCheck = findObjectAtCoordinate(&list, rightCoor);
            switch (rightCheck)
            {
            case 0:
                printf("avoiding right collision \n");
                avoidCollisions(0,current,destination, xDistanceToCover, yDistanceToCover);
                break;
            case 1:
                current = moveRobot(current, 1);
                break;
            }
        }
    } else if (xDistanceToCover < 0) {
        printf("Covering x distance left \n");
        while (current.x != destination.x) {
            struct coordinates leftCoor = current; 
            leftCoor.x = current.x - 1;
            int leftCheck = findObjectAtCoordinate(&list, leftCoor);
            switch (leftCheck)
            {
            case 0:
                printf("avoiding left collision \n");
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
    printf("CliffDistance = %d \n", cliffDistance);
    //distance of each move is assumed to be 5 cm 
    if (cliffDistance < 50) {
        s.cliffPresent = 1;
    } else {
        s.cliffPresent = 0;
    }
    
    printf("cliffPresent = %d \n", s.cliffPresent);

    IR = measureIRData();
    printf("IR sensor value 1,2,3,4 = %d, %d, %d, %d \n", IR.sensor1Val, IR.sensor2Val, IR.sensor3Val, IR.sensor4Val);
    if (IR.sensor1Val < IRthreshold && IR.sensor2Val < IRthreshold && IR.sensor3Val < IRthreshold && IR.sensor4Val < IRthreshold) {
        s.boundaryPresent = 1; //if all four sensors indicate the surface is black we return true for boundary present
        s.holePresent = 0;
    } else if (IR.sensor1Val < IRthreshold || IR.sensor2Val < IRthreshold || IR.sensor3Val < IRthreshold || IR.sensor4Val < IRthreshold) {
        s.boundaryPresent = 0; 
        s.holePresent = 1;
    } else {
        s.boundaryPresent = 0;
        s.holePresent = 0;
    }

    printf("boundary present = %d \n", s.boundaryPresent);
    printf("hole present = %d \n", s.holePresent);

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

    printf("blockType = %d \n", s.blockType);
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
    int property = 0;
    printf("returnSquarePropety boundary = %d \n", s.boundaryPresent);
    if(s.boundaryPresent == 1) {
        property = 1; //1 represents boundary
    } else if (s.cliffPresent == 1) {
        property = 2; // 2 represents cliff
    } else if (s.holePresent == 1) {
        property = 3; // 3 represents hole
    } else if (s.blockType != 6) {
        property = 10*s.blockType + s.blockColor;
    } 
    
    printf("RETURNSQUAREPROPERTY PROPERTY = %d \n", property);
        return property;
}

/**
 * method to explore the square ahead of the currently occupied square
 * adds the coordinate explored to the array
 * cc represents the current coordinates and be represents the coordinate being explored
*/
int exploreForward(struct coordinates cc, struct Queue* queue) {
    printf("forward exploration started\n");

    int check = 0;
    struct coordinates be = cc;
    be.y = cc.y + 1;
    int duplication_check = isInArrayList(&list, be); 
    if(duplication_check > 0) { 
        check = 1; 
        printf("duplicate found in arraylist\n");
        return check; 
    }
    struct squareType st = checkSquare();
    int propertyAtCoordinate;
    propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate; // add the value for the object at location to the coordinate before adding to list 
    printf("property at coordinate: %d \n", propertyAtCoordinate);
    if (propertyAtCoordinate == 0) { //0 represents empty
        printf("forward enqueue condition passed\n");
        enqueue(queue, be);    //coordinate is only enqueued for further exploration if it is empty.
    }
    addElement(&list, be);
    return check;
}

/**
 * method to explore the square to the right of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreRight(struct coordinates cc, struct Queue* queue) {
    printf("RIGHT EXPLORATION STARTED\n");
    int check = 0;
    struct coordinates be = cc;
    be.x = cc.x + 1;
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
        printf("duplicate found in arraylist \n");
        return check;
    }
    rotateRobot(0);
    struct squareType st = checkSquare();
    int propertyAtCoordinate;
    propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    
    if (propertyAtCoordinate == 0) {
        printf("right exploration condition passed \n");
        enqueue(queue, be);
    }
    addElement(&list, be);
    rotateRobot(1);
    return check;
}

/**
 * method to explore the square to the left of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreLeft(struct coordinates cc, struct Queue* queue) {
    printf("LEFT EXPLORATION STARTED\n");
    int check = 0;
    struct coordinates be = cc;
    be.x = cc.x - 1; 
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
        printf("duplicate found in arraylist\n");
        return check;
    }
    rotateRobot(1);
    struct squareType st = checkSquare();
    int propertyAtCoordinate;
    propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    if (propertyAtCoordinate == 0) {
        printf("left exploration condition passed \n");
        enqueue(queue, be);
    }
    addElement(&list, be);
    rotateRobot(0);
    return check;
}

/**
 * method to explore the square behind of the currently occupied square
 * adds the coordinate explored to the array
*/
int exploreBehind(struct coordinates cc, struct Queue* queue) {
    printf("BACKWARD EXPLORATION STARTED\n");
    int check = 0;
    struct coordinates be = cc;
    be.y = cc.y - 1;
    int duplication_check = isInArrayList(&list, be);
    if(duplication_check > 0) {
        check = 1;
        printf("duplicate found in arraylist\n");
        return check;
    }
    rotateRobot(2);
    struct squareType st = checkSquare();
    int propertyAtCoordinate;
    propertyAtCoordinate = returnSquareProperty(st);
    be.objectAtLocation = propertyAtCoordinate;
    
    if (propertyAtCoordinate == 0) {
        printf("backward exploration condition passed \n");
        enqueue(queue, be);
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

 /**
 * BFS search algorithm to search to explore the entire grid
*/
void BFS(struct coordinates currentCoordinate) {
    int i = 0;
    int duplication_check = 0;
    struct coordinates beingExplored = currentCoordinate; 
    struct coordinates navigateTo;
    struct Queue* queue = createQueue(1000);   //create a BFS queue
   
    printf("BFS STARTED\n");

    //while loop which runs until a duplicate is found or until the time limit has been reached
    while (duplication_check < 1 || i < 5) {
        printf("Cooridnate BeingExplored = (%d, %d, %d)\n", beingExplored.x, beingExplored.y, beingExplored.objectAtLocation);
        //explore all of the squares adjacent to currently occupied square
        duplication_check = exploreForward(beingExplored, queue);
        duplication_check = duplication_check + exploreRight(beingExplored, queue);
        duplication_check = duplication_check + exploreLeft(beingExplored, queue);
        duplication_check = duplication_check + exploreBehind(beingExplored, queue); //if duplication_check > 0 break while loop


        navigateTo = dequeue(queue);        //dequeue one of the visited coordinates from the queue and 
        //add it to a new struct called coordinates
        printf("coordinates (%d, %d, %d) dequeued from queue for Navigation\n\n", navigateTo.x, navigateTo.y, navigateTo.objectAtLocation);
        robotNavigation(beingExplored, navigateTo);
        beingExplored = navigateTo; //once the robotNavigation() method has been executed set the robot's beingExplored coordinate to the navigateTo coordinate. 
        i++; 
        
    }

    printf("Duplicate has been found\n");
    
}


