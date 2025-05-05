#include <algorithm>
#include <filesystem>
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
    prepareRead();
    position++;
    int x;
    file.read(reinterpret_cast<char*>(&x), sizeof(x));
    return x;
}

void FileTape::write(int x) {
    prepareWrite();
    position++;
    file.write(reinterpret_cast<char*>(&x), sizeof(x));
}

void FileTape::move(int add) {
    position += add;
    file.seekg(position);
}

int FileTape::size() {
    return std::filesystem::file_size(filename) / sizeof(int);
}

void FileTape::flush() {
    file.flush();
}

void FileTape::clear() {
    file.close();
    std::filesystem::remove(filename);
    file.open(filename, std::ios::binary | std::ios::out);
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

int FileTape::getPosition() {
    return position;
}

StatTape::StatTape(std::string filename, int rwTime, int moveTime, int resetTime) {
    tape = new FileTape(filename);
    this->rwTime = rwTime;
    this->moveTime = moveTime;
    this->resetTime = resetTime;
    time = 0;
}

StatTape::~StatTape() {
    delete tape;
}

int StatTape::read() {
    if (!tape->isReading()) {
        time += std::min(resetTime, tape->getPosition() * moveTime);
    }
    time += rwTime + moveTime;
    return tape->read();
}

void StatTape::write(int x) {
    if (tape->isReading()) {
        time += std::min(resetTime, tape->getPosition() * moveTime);
    }
    time += rwTime + moveTime;
    tape->write(x);
}

void StatTape::move(int add) {
    time += std::abs(add) * moveTime;
    tape->move(add);
}

int StatTape::size() {
    return tape->size();
}

void StatTape::flush() {
    tape->flush();
}

void StatTape::clear() {
    tape->clear();
}

int StatTape::getTime() {
    return time;
}

void StatTape::resetTimer() {
    time = 0;
}