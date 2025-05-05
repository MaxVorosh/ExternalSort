#include <iostream>
#include "tape.h"
#include "externalSort.h"

int main() {
    std::string filename = "../examples/example";
    std::string outName = "../examples/out_example";
    std::string tmpPath1 = "../tmp/1";
    std::string tmpPath2 = "../tmp/2";
    std::string tmpPath3 = "../tmp/3";
    {   
        StatTape ft = StatTape(filename, 1, 10, 100);
        ft.write(1000);
        ft.write(10);
        ft.write(-1);
    }
    StatTape in = StatTape(filename, 1, 10, 100);
    StatTape out = StatTape(outName, 1, 10, 100);
    StatTape tmp1 = StatTape(tmpPath1, 1, 10, 100);
    StatTape tmp2 = StatTape(tmpPath2, 1, 10, 100);
    StatTape tmp3 = StatTape(tmpPath3, 1, 10, 100);
    // std::cout << in.size() << std::endl;
    // std::cout << out.size() << std::endl;
    // std::cout << tmp.size() << std::endl;
    // std::cout << "Start" << std::endl;
    std::vector<Tape*> tmpTapes = {&tmp1, &tmp2, &tmp3};
    FastSorter sorter(10);
    sorter.sort(&in, &out, tmpTapes);
    StatTape reader = StatTape(outName, 1, 10, 100);
    for (int i = 0; i < reader.size(); ++i) {
        std::cout << reader.read() << ' ';    
    }
    std::cout << std::endl;
    // std::cout << in.size() << std::endl;
    // std::cout << tmp.size() << std::endl;
    // std::cout << reader.size() << std::endl;
    std::cout << in.getTime() + out.getTime() + tmp1.getTime() + tmp2.getTime() + tmp3.getTime() << std::endl;
}