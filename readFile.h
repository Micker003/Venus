// Read the color.txt file and return the integer value stored in the file as an integer

#ifndef READFILE_H
#define READFILE_H

#include <iostream>
#include <fstream>
#include <string>

int readIntegerFromFile() {
    std::ifstream file("state.txt");
    if (!file) {
        std::cerr << "Error opening file\n";
        return -1;
    }

    int number;
    file >> number;

    file.close();

    return number;
}

#endif // READFILE_H
