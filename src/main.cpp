#include <iostream>
#include "tape.h"
#include "externalSort.h"

void printUsage() {
    std::cout << "Usage: ./ExternalSort <input file path> <output file path> <memory limit>" << std::endl;
    std::cout << "Optional flags:" << std::endl;
    std::cout << "-rw <number> : Timeout for reading/writing number from tape. Default 1" << std::endl;
    std::cout << "-move <number> : Timeout for moving head for one space. Default 10" << std::endl;
    std::cout << "-reset <number> : Timeout for moving tape more than one space. Default 100" << std::endl;
    std::cout << "-type <number> : Type of sort. 1 is slow sort with lower memory usage, 2 is fast sort with more memory usage. Default 2" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 3 || argc % 2 == 1) {
        printUsage();
        return 0;
    }
    std::string inName = argv[1];
    std::string outName = argv[2];
    int memoryLimit = std::atoi(argv[3]) / sizeof(int);
    int rw = 1;
    int move = 10;
    int reset = 100;
    int type = 2;
    for (int i = 4; i < argc; i+=2) {
        std::string key = argv[i];
        int val = std::atoi(argv[i + 1]);
        if (key == "-rw") {
            rw = val;
        }
        else if (key == "-move") {
            move = val;
        }
        else if (key == "-reset") {
            reset = val;
        }
        else if (key == "-type") {
            type = val;
        }
        else {
            printUsage();
            return 0;
        }
    }
    rw = (rw + move) * sizeof(int);

    std::string tmpPath1 = "../tmp/1";
    std::string tmpPath2 = "../tmp/2";

    StatTape in = StatTape(inName, rw, move, reset);
    StatTape out = StatTape(outName, rw, move, reset);
    StatTape tmp1 = StatTape(tmpPath1, rw, move, reset);
    StatTape tmp2 = StatTape(tmpPath2, rw, move, reset);
    std::vector<Tape*> tmpTapes = {&tmp1, &tmp2};

    if (type == 1) {
        SimpleSorter sorter(memoryLimit);
        sorter.sort(&in, &out, tmpTapes);
    }
    else {
        FastSorter sorter(memoryLimit);
        sorter.sort(&in, &out, tmpTapes);
    }
    std::cout << "Time: " << in.getTime() + out.getTime() + tmp1.getTime() + tmp2.getTime() << std::endl;
}