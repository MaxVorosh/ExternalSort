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
    file.seekg(position * sizeof(int));
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
    file.seekg(position * sizeof(int));
}

void FileTape::prepareWrite() {
    if (!forReading) {
        return;
    }
    forReading = false;
    file.close();
    file.open(filename, std::ios::binary | std::ios::out);
    file.seekg(position * sizeof(int));
}

bool FileTape::isReading() {
    return forReading;
}

int FileTape::getPosition() {
    return position;
}

StatTape::StatTape(std::string filename, int rwTime, int moveTime, int resetTime) {
    tape = new FileTape(filename);
    this->rwTime = rwTime; // time for reading/writing int, including moves of head of tape
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
    time += rwTime;
    return tape->read();
}

void StatTape::write(int x) {
    if (tape->isReading()) {
        time += std::min(resetTime, tape->getPosition() * moveTime);
    }
    time += rwTime;
    tape->write(x);
}

void StatTape::move(int add) {
    time += std::min(std::abs(add) * moveTime, resetTime);
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