#include <fstream>
#include <string>

#pragma once

class Tape {
public:
    virtual int read() = 0;
    virtual void write(int x) = 0;
    virtual void move(int add) = 0;

    virtual int size() = 0;
    virtual void flush() = 0;
    virtual void clear() = 0;
};

class FileTape : public Tape {
public:
    FileTape(std::string filename);
    ~FileTape();

    int read() override;
    void write(int x) override;
    void move(int add) override;

    int size();
    void flush();
    void clear();

    void prepareRead();
    void prepareWrite();

    bool isReading();
    int getPosition();
private:
    int position;
    std::fstream file;
    bool forReading;
    std::string filename;
};

class StatTape : public Tape {
public:
    StatTape(std::string filename, int rwTime, int moveTime, int resetTime);
    ~StatTape();

    int read() override;
    void write(int x) override;
    void move(int add) override;

    int size();
    void flush();
    void clear();

    int getTime();
    void resetTimer();
private:
    int rwTime;
    int moveTime;
    int resetTime;
    FileTape* tape;
    int time;
};