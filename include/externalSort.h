#include <string>

#pragma once

class ExternalSorter {
public:
    ExternalSorter(int memSize);
    virtual void sort(std::string inName, std::string outName) = 0;
private:
    int memSize;
};

class SimpleSorter : public ExternalSorter {
public:
    SimpleSorter(int memSize);
    void sort(std::string inName, std::string outName) override;
};

class FastSorter : public ExternalSorter {
public:
    FastSorter(int memSize);
    void sort(std::string inName, std::string outName) override;
};