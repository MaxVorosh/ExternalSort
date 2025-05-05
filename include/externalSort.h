#include <string>
#include <vector>
#include "tape.h"

#pragma once

class ExternalSorter {
public:
    ExternalSorter(int memSize);
    virtual void sort(Tape* inTape, Tape* outTape, std::vector<Tape*>& tmpTapes) = 0;
protected:
    int memSize;

    void readBuffer(Tape* tape, std::vector<int>& buf, int size);
    void writeBuffer(Tape* tape, std::vector<int>& buf, int size);
    void merge(Tape* in, Tape* out, std::vector<int>& buf);
};

class SimpleSorter : public ExternalSorter {
public:
    SimpleSorter(int memSize);
    void sort(Tape* inTape, Tape* outTape, std::vector<Tape*>& tmpTapes) override;
};

class FastSorter : public ExternalSorter {
public:
    FastSorter(int memSize);
    void sort(Tape* inTape, Tape* outTape, std::vector<Tape*>& tmpTapes) override;
};