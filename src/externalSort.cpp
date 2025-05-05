#include "externalSort.h"

ExternalSorter::ExternalSorter(int memSize) {
   this->memSize = memSize; 
}

SimpleSorter::SimpleSorter(int memSize) : ExternalSorter(memSize) {}

void SimpleSorter::sort(std::string inName, std::string outName) {
    //TODO write sort
}

FastSorter::FastSorter(int memSize) : ExternalSorter(memSize) {}

void FastSorter::sort(std::string inName, std::string outName) {
    //TODO write sort
}