// Read the color.txt file and return the integer value stored in the file as an integer
// This file is written in C 

#include <string.h>
#include <stdio.h>

int readIntegerFromFile() {
    FILE *file = fopen("color.txt", "r");
    if (!file) {
        printf("Error opening file\n");
        return -1;
    }

    int number;
    fscanf(file, "%d", &number);

    fclose(file);

    return number;
}

// int readIntegerFromFile() {
//     std::ifstream file("state.txt");
//     if (!file) {
//         std::cerr << "Error opening file\n";
//         return -1;
//     }

//     int number;
//     file >> number;

//     file.close();

//     return number;
// }

// #endif // READFILE_H