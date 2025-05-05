#include <iostream>
#include "tape.h"
#include "externalSort.h"

int main() {
    std::string filename = "../examples/example";
    std::string outName = "../examples/out_example";
    std::string tmpPath = "../tmp/1";
    {   
        StatTape ft = StatTape(filename, 1, 10, 100);
        ft.write(1000);
        ft.write(10);
        ft.write(-1);
    }
    StatTape in = StatTape(filename, 1, 10, 100);
    StatTape out = StatTape(outName, 1, 10, 100);
    StatTape tmp = StatTape(outName, 1, 10, 100);
    // std::cout << in.size() << std::endl;
    // std::cout << out.size() << std::endl;
    // std::cout << tmp.size() << std::endl;
    // std::cout << "Start" << std::endl;
    std::vector<Tape*> tmpTapes = {&tmp};
    SimpleSorter sorter(1);
    sorter.sort(&in, &out, tmpTapes);
    StatTape reader = StatTape(outName, 1, 10, 100);
    for (int i = 0; i < reader.size(); ++i) {
        std::cout << reader.read() << ' ';    
    }
    std::cout << std::endl;
    // std::cout << in.size() << std::endl;
    // std::cout << tmp.size() << std::endl;
    // std::cout << reader.size() << std::endl;
    std::cout << in.getTime() + out.getTime() + tmp.getTime() << std::endl;
}