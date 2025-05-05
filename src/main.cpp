#include <iostream>
#include "tape.h"

int main() {
    std::string filename = "../examples/example";
    FileTape ft = FileTape(filename);
    ft.prepareWrite();
    ft.write(1000);
    ft.prepareRead();
    int x = ft.read();
    std::cout << x <<std::endl;
}