#ifndef ROBOTALGORITHMBFSTEST_H
#define ROBOTALGORITHMBFSTEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

//struct for the properties of a particular square as interpreted by sensor data
struct squareType {
    //blockType can be 4 for "small" || 5 for "big" || 6 for null
    int blockType; 
    //block color can be any string corresponding to the RBG color returned by embedded software method the integer represents the rgb value
    //parity value, 1 is true and 0 is false
    int cliffPresent;
    //parity value for hole
    int holePresent;
    //parity value for boundary 
    int boundaryPresent; 

};


struct Queue* createQueue(unsigned capacity);
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, struct coordinates item);
struct coordinates dequeue(struct Queue* queue);
struct coordinates front(struct Queue* queue);
struct coordinates rear(struct Queue* queue);

int isInArrayList(ArrayList *list, struct coordinates target);
void addElement(ArrayList *list, struct coordinates element);
void initArrayList(ArrayList *list);


struct coordinates moveRobot(struct coordinates inputCoordinate, int dirSelVal);
int findObjectAtCoordinate(ArrayList *list, struct coordinates target);
void avoidCollisions(int axisOfMovement, struct coordinates current, struct coordinates target, int xDistanceToCover, int yDistanceToCover);
void robotNavigation(struct coordinates current, struct coordinates destination);
int convertToColor(struct color color);
struct squareType checkSquare();
    
int returnSquareProperty(struct squareType s);
int exploreForward(struct coordinates cc, struct Queue* queue);
int exploreRight(struct coordinates cc, struct Queue* queue);
int exploreLeft(struct coordinates cc, struct Queue* queue);
int exploreBehind(struct coordinates cc, struct Queue* queue);
void BFS(struct coordinates currentCoordinate);


#endif // ROBOTALGORITHMBFSTEST_H
