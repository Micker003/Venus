#include "BFStest.c"
#include "embeddedInitialImp.c"

/**
 * File with test cases to test each of the methods inside of the algorithm.
 * For each test method specify input and expected output in comments above method. 
 * run a test for each method and ensure that the method performs within the use case. 
 * create test cases to verify the arraylist and queue as well. 
*/

 while (!(current.y == destination.y)) {
        struct coordinates fwd = current; 
        fwd.y = current.y+1;
        int fcheck = findObjectAtCoordinate(&list, fwd);
        struct coordinates bwd = current; 
        bwd.y = current.y-1;
        int bcheck = findObjectAtCoordinate(&list, bwd);
        if (yDistanceToCover > 0 && fcheck == 1) {
            //if y distance to cover is positive then we must move up 
            current = moveRobot(current, 0);
    }  else if (yDistanceToCover > 0 && fcheck == 0) {
            if (xDistanceToCover > 0) {
                current = moveRobot(current, 1); //if the forward coordinate isn't empty we move one to the right
            } else {
                current = moveRobot(current, 2); //if it cannot go right then move to the left. 
            }
            
    }  
    
        if (yDistanceToCover < 0 && bcheck == 1) {
            //if y distance to cover is negative then we must move down 
            current = moveRobot(current, 3);
        } else if (yDistanceToCover < 0 && bcheck == 0) {
            if (xDistanceToCover > 0) {
                current = moveRobot(current, 1); //if the forward coordinate isn't empty we move one to the right
            } else {
                current = moveRobot(current, 2); //if it cannot go right then move to the left. 
            }
        }
    }
    
    while (!(current.x == destination.x)) {
        struct coordinates right = current; 
        right.x = current.x+1;
        int Rcheck = findObjectAtCoordinate(&list, right);
        struct coordinates left = current; 
        left.x = current.x-1;
        int Lcheck = findObjectAtCoordinate(&list, left);

        if (xDistanceToCover > 0 && Rcheck == 1) {
            //if y distance to cover is positive then we must move up 
            current = moveRobot(current, 1);
    }  else if (xDistanceToCover > 0 && Rcheck == 0) {
            if (yDistanceToCover > 0) {
                current = moveRobot(current, 3); //if the Right coordinate isn't empty we move one down
            } else {
                current = moveRobot(current, 0); //if it cannot go down then we move down
            }
            
    }  
    
        if (xDistanceToCover < 0 && Lcheck == 1) {
            //if y distance to cover is negative then we must move down 
            current = moveRobot(current, 2);
        } else if (yDistanceToCover < 0 && bcheck == 0) {
            if (xDistanceToCover > 0) {
                current = moveRobot(current, 1); //if the forward coordinate isn't empty we move one to the right
            } else {
                current = moveRobot(current, 2); //if it cannot go right then move to the left. 
            }
        }
    }