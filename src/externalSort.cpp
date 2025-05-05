#include <algorithm>
#include <iostream>
#include "externalSort.h"

ExternalSorter::ExternalSorter(int memSize) {
   this->memSize = memSize; 
}

void ExternalSorter::readBuffer(Tape* tape, std::vector<int>& buf, int size) {
    for (int i = 0; i < size; ++i) {
        buf[i] = tape->read();
    }
}

void ExternalSorter::writeBuffer(Tape* tape, std::vector<int>& buf, int size) {
    for (int i = 0; i < size; ++i) {
        tape->write(buf[i]);
    }
    tape->flush();
    std::cout << tape->size() << ' ' << size << std::endl;
}

void ExternalSorter::merge(Tape* in, Tape* out, std::vector<int>& buf) {
    int bufSize = buf.size();
    int inSize = in->size();
    if (inSize == 0) {
        writeBuffer(out, buf, bufSize);
        return;
    }
    int l = 0;
    int inElem = in->read();
    int r = 0;
    while (l < inSize || r < bufSize) {
        if (l < inSize && inElem < bufSize || r == bufSize) {
            out->write(inElem);
            l++;
            if (l < inSize) {
                inElem = in->read();
            }
        }
        else {
            out->write(buf[r]);
            r++;
        }
    }
    out->flush();
}

SimpleSorter::SimpleSorter(int memSize) : ExternalSorter(memSize) {}

void SimpleSorter::sort(Tape* inTape, Tape* outTape, std::vector<Tape*>& tmpTapes) {
    if (tmpTapes.size() == 0) {
        throw std::runtime_error("Not enough tapes");
    }
    Tape* tmp = tmpTapes[0];
    tmp->clear();
    outTape->clear();
    std::vector<int> buf = std::vector<int>(memSize);
    Tape* first = outTape;
    Tape* second = tmp;
    int moved = 0;
    int inSize = inTape->size();
    for (int i = 0; i < inSize; i+=memSize) {
        int readSize = std::min(memSize, inSize - i);
        buf.resize(readSize);
        readBuffer(inTape, buf, readSize);
        std::sort(buf.begin(), buf.end());
        merge(first, second, buf); // always writing in second
        std::swap(first, second);
    }
    if (outTape != first) {
        buf.clear();
        merge(tmp, outTape, buf);
    } 
}

FastSorter::FastSorter(int memSize) : ExternalSorter(memSize) {}

void FastSorter::sort(Tape* inTape, Tape* outTape, std::vector<Tape*>& tmpTapes) {
    //TODO write sort
}