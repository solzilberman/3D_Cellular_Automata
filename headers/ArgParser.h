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
    cout << "  ./main --animation true --rotation true --rulenum 10" << endl;
    cout << "  ./main --rulenum 10 --sidelength 20" << endl;
    cout << "  ./main -a true -r false -rn 8" << endl;
    cout << "Options:" << endl;
    cout << "  -h    --help        Enable\\Disable Animation" << endl;
    cout << "  -a    --animation   Enable\\Disable Animation" << endl;
    cout << "  -r    --rotation    Enable\\Disable Rotation" << endl;
    cout << "  -rn   --rulenum     CA rule number [0-10]" << endl;
    cout << "  -s    --sidelength  Length of side of CUBE-WORLD" << endl;
    exit(1);
};

#endif // ARGPARSER_H
