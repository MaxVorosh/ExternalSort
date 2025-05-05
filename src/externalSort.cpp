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
    inTape->move(-inSize);
}

void SimpleSorter::merge(Tape* in, Tape* out, std::vector<int>& buf) {
    int bufSize = buf.size();
    int inSize = in->size();
    if (inSize == 0) {
        writeBuffer(out, buf, bufSize);
        out->move(-inSize - bufSize);
        return;
    }
    int l = 0;
    int inElem = in->read();
    int r = 0;
    while (l < inSize || r < bufSize) {
        if (l < inSize && inElem < buf[r] || r == bufSize) {
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
    in->move(-inSize);
    out->flush();
    out->move(-inSize - bufSize);
}

FastSorter::FastSorter(int memSize) : ExternalSorter(memSize) {}

void FastSorter::sort(Tape* inTape, Tape* outTape, std::vector<Tape*>& tmpTapes) {
    if (tmpTapes.size() < 3) {
        throw std::runtime_error("Not enough tapes");
    }
    outTape->clear();
    for (int i = 0; i < 3; ++i) {
        tmpTapes[i]->clear();
    }
    Tape* firstReader = tmpTapes[0];
    Tape* secondReader = tmpTapes[1];
    Tape* writer = tmpTapes[2];
    std::vector<int> buf = std::vector<int>(memSize);
    int inSize = inTape->size();
    for (int i = 0; i < inSize; i+=memSize) {
        int readSize = std::min(memSize, inSize - i);
        buf.resize(readSize);
        readBuffer(inTape, buf, readSize);
        std::sort(buf.begin(), buf.end());
        writeBuffer(firstReader, buf, readSize);
        writeBuffer(secondReader, buf, readSize);
    }
    firstReader->move(-inSize);
    secondReader->move(-inSize);
    mergeSortedBlocks(firstReader, secondReader, writer, outTape);
    inTape->move(-inSize);
}

void FastSorter::mergeSortedBlocks(Tape* firstReader, Tape* secondReader, Tape* writer, Tape* outTape) {
    int blockSize = memSize;
    int tSize = firstReader->size();
    while (blockSize < tSize) {
        mergeMany(firstReader, secondReader, writer, blockSize);
        blockSize *= 2;
        firstReader->move(-tSize);
        secondReader->move(-tSize);
        writer->move(-tSize);
        for (int i = 0; i < tSize; ++i) {
            firstReader->write(writer->read());
        }
        firstReader->flush();
        firstReader->move(-tSize);
        writer->move(-tSize);
        std::swap(secondReader, writer);
    }
    for (int i = 0; i < tSize; ++i) {
        outTape->write(firstReader->read());
    }
    outTape->flush();
    outTape->move(-tSize);
    firstReader->move(-tSize);
}

void FastSorter::mergeMany(Tape* firstReader, Tape* secondReader, Tape* writer, int blockSize) {
    int tSize = firstReader->size();
    int l = 0;
    int r = std::min(blockSize, tSize);
    secondReader->move(r);
    while (l < tSize || r < tSize) {
        int edgeL = std::min(tSize, l + blockSize);
        int edgeR = std::min(tSize, r + blockSize);
        mergeBlock(firstReader, secondReader, writer, l, r, edgeL, edgeR);
        l = edgeL;
        r = edgeR;
        int addL = std::min(blockSize, tSize - l);
        int addR = std::min(blockSize, tSize - r);
        l += addL;
        r += addR;
        firstReader->move(addL);
        secondReader->move(addR);
    }
}

void FastSorter::mergeBlock(Tape* firstReader, Tape* secondReader, Tape* writer, int l, int r, int edgeL, int edgeR) {
    int lValue, rValue;
    if (l < edgeL) {
        lValue = firstReader->read();
    }
    if (r < edgeR) {
        rValue = secondReader->read();
    }
    while (l < edgeL || r < edgeR) {
        if (l == edgeL || (r < edgeR && lValue > rValue)) {
            writer->write(rValue);
            r++;
            if (r < edgeR) {
                rValue = secondReader->read();
            }
        }
        else {
            writer->write(lValue);
            l++;
            if (l < edgeL) {
                lValue = firstReader->read();
            }
        }
    }
    writer->flush();
}