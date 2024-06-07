#include "embeddedImplementationInterface.c"
#include "BFSTest.c"

/**
 * fieldSplitter class to split the field in half annd distribute the two halves to the two robots. 
*/

struct coordinates startingCoordinate = {0,0,0};

void coverExtremities() {
    printf("Covering Y distance up \n");
    int property_At_Coordinate = 0;
        while (property_At_Coordinate == 0) {
            struct coordinates upCoor = current; 
            upCoor.y = current.y + 1;
            int upCheck = findObjectAtCoordinate(&list, upCoor);
            switch (upCheck)
            {
            case 0:
                printf("avoiding up collision \n");
                avoidCollisions(1, current, destination, 0, 100);  //if a collision is detected then avoid collision method is called
                break;
            case 1:
                current = moveRobot(current, 0);  //if the coordinate is empty then we move the robot up. 
                break;    
            default:
                break;
            }
        }
    while() {
        moveRobot(startingCoordinate, 0);
    }

}

int main() {

    
    return 0; 
}