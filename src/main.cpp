#include <iostream>
#include "tape.h"

int main() {
    std::string filename = "../examples/example";
    StatTape ft = StatTape(filename, 1, 10, 100);
    ft.write(1000);
    int x = ft.read();
    std::cout << x <<std::endl;
    std::cout << ft.getTime() << std::endl;
}