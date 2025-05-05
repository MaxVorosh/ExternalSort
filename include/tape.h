#include <fstream>
#include <string>

#pragma once

class Tape {
public:
    virtual int read() = 0;
    virtual void write(int x) = 0;
    virtual void move(int add) = 0;
};

class FileTape : public Tape {
public:
    FileTape(std::string filename);
    ~FileTape();

    int read() override;
    void write(int x) override;
    void move(int add) override;

    void prepareRead();
    void prepareWrite();
    bool isReading();
private:
    int position;
    std::fstream file;
    bool forReading;
    std::string filename;
};