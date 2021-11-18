/*
 @author - Sol Zilberman
 */
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <algorithm>

#ifndef ARGPARSER_H
#define ARGPARSER_H

char *getCmdOption(char **begin, char **end, const std::string &option) {
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    cout << option;
    return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option) {
    return std::find(begin, end, option) != end;
}

void printHelpMenu() {
    cout << "Welcome to 3D CA in OpenGL :)" << endl;
    cout << "Usage:" << endl;
    cout << "  ./main [@bool animation enabled] [@bool rotation enabled] [@int rule number] [@int "
            "side length] [@int animstep]"
         << endl;
    cout << "  ./main [@bool animation enabled] [@bool rotation enabled] [@int rule number] [@int "
            "side length]"
         << endl;
    cout << "  ./main [@bool animation enabled] [@bool rotation enabled] [@int rule number]"
         << endl;
    cout << "  ./main [@bool animation enabled] [@bool rotation enabled]" << endl;
    cout << "  ./main [@int rule number]" << endl;
    cout << "  ./main" << endl;
    cout << "Example Usage:" << endl;
    cout << "  ./main true false 10" << endl;
    cout << "  ./main 6" << endl;
    cout << "  ./main true true 10 20 5" << endl;
    exit(1);
};

#endif // ARGPARSER_H
