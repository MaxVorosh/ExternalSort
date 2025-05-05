#include <iostream>
#include "tape.h"

FileTape::FileTape(std::string filename) {
    position = 0;
    this->filename = filename;
    file.open(filename, std::ios::in | std::ios::binary);
    forReading = true;
}

FileTape::~FileTape() {
    file.close();
}

int FileTape::read() {
    if (!forReading) {
        throw std::runtime_error("File open for writing");
    }
    int x;
    file >> x;
    return x;
}

void FileTape::write(int x) {
    if (forReading) {
        throw std::runtime_error("File open for reading");
    }
    file << x;
    file.flush();
}

void FileTape::move(int add) {
    position += add;
    file.seekg(position);
}

void FileTape::prepareRead() {
    if (forReading) {
        return;
    }
    forReading = true;
    file.close();
    file.open(filename, std::ios::binary | std::ios::in);
    position = 0;
}

void FileTape::prepareWrite() {
    if (!forReading) {
        return;
    }
    forReading = false;
    file.close();
    file.open(filename, std::ios::binary | std::ios::out);
    position = 0;
}

bool FileTape::isReading() {
    return forReading;
}