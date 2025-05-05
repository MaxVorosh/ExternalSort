#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "tape.h"
#include "externalSort.h"

#include <fstream>
#include <vector>

std::string smallExample = "../test/test_data/small_example";
std::string largeExample = "../test/test_data/large_example";
std::string zeroExample = "../test/test_data/zero_example";
std::string oneExample = "../test/test_data/one_example";
std::string writeExample = "../test/test_data/write_example";
std::string tmp1Example = "../test/test_data/tmp/1";
std::string tmp2Example = "../test/test_data/tmp/2";
std::string tmp3Example = "../test/test_data/tmp/3";

void initFile(std::string name, std::vector<int>& values) {
    std::ofstream fout(name, std::ios::binary);
    for (int val : values) {
        fout.write(reinterpret_cast<char*>(&val), sizeof(val));
    }
    fout.close();
}

void initTests() {
    static bool wasInit = false;
    if (wasInit) {
        return;
    }
    wasInit = true;
    std::vector<int> small_data = {1000, -1, 10};
    std::vector<int> zero_data = {};
    std::vector<int> one_data = {1};
    std::vector<int> large_data(10000);
    for (int i = 0; i < large_data.size(); ++i) {
        large_data[i] = large_data.size() - i;
    }
    initFile(smallExample, small_data);
    initFile(largeExample, large_data);
    initFile(zeroExample, zero_data);
    initFile(oneExample, one_data);
}

void checkSorter(std::string filename, int memoryLimit, bool isSimple) {
    FileTape inFile(filename);
    FileTape outFile(writeExample);
    FileTape tmp1File(tmp1Example);
    FileTape tmp2File(tmp2Example);
    FileTape tmp3File(tmp3Example);
    std::vector<Tape*> tapes = {&tmp1File, &tmp2File, &tmp3File};

    std::vector<int> data(inFile.size());
    for (int i = 0; i < data.size(); ++i) {
        data[i] = inFile.read();
    }
    inFile.move(-data.size());

    if (isSimple) {
        SimpleSorter sorter(memoryLimit);
        sorter.sort(&inFile, &outFile, tapes);
    }
    else {
        FastSorter sorter(memoryLimit);
        sorter.sort(&inFile, &outFile, tapes);
    }

    REQUIRE(inFile.getPosition() == 0);
    REQUIRE(inFile.size() == data.size());
    REQUIRE(outFile.getPosition() == 0);
    REQUIRE(outFile.size() == data.size());

    REQUIRE(tmp1File.getPosition() == 0);
    REQUIRE(tmp2File.getPosition() == 0);
    REQUIRE(tmp3File.getPosition() == 0);

    for (int i = 0; i < data.size(); ++i) {
        CHECK(data[i] == inFile.read());
    }
    std::sort(data.begin(), data.end());
    for (int i = 0; i < data.size(); ++i) {
        CHECK(data[i] == outFile.read());
    }
}

TEST_CASE("FileTape read test") {
    initTests();
    FileTape file(smallExample);
    CHECK(file.read() == 1000);
    CHECK(file.read() == -1);
    CHECK(file.read() == 10);
}

TEST_CASE("FileTape write test") {
    FileTape fileT(writeExample);
    fileT.write(32);
    fileT.write(239);
    fileT.flush();
    std::ifstream file(writeExample, std::ios::binary);
    int x;
    file.read(reinterpret_cast<char*>(&x), sizeof(int));
    CHECK(x == 32);
    file.read(reinterpret_cast<char*>(&x), sizeof(int));
    CHECK(x == 239);
}

TEST_CASE("FileTape size test") {
    FileTape smallTape(smallExample);
    CHECK(smallTape.size() == 3);
    FileTape largeTape(largeExample);
    CHECK(largeTape.size() == 10000);
    FileTape oneTape(oneExample);
    CHECK(oneTape.size() == 1);
    FileTape zeroTape(zeroExample);
    CHECK(zeroTape.size() == 0);
}

TEST_CASE("FileTape move test") {
    FileTape largeTape(largeExample);
    int pos = 0;
    int adds[] = {100, -50, 23, -42, 0};
    for (int i = 0; i < 5; ++i) {
        pos += adds[i];
        largeTape.move(adds[i]);
        int x = largeTape.read();
        CHECK(x + pos == 10000);
        pos += 1;
        CHECK(pos == largeTape.getPosition());
    }
}

TEST_CASE("FileTape read-write test") {
    FileTape tape(writeExample);
    tape.write(1);
    tape.write(2);
    tape.write(3);
    tape.flush();
    tape.move(-2);
    CHECK(tape.read() == 2);
}

TEST_CASE("FileTape clear test") {
    FileTape tape(writeExample);
    tape.write(1);
    tape.write(2);
    tape.flush();
    CHECK(tape.size() == 2);
    tape.clear();
    CHECK(tape.size() == 0);
}


TEST_CASE("StatTape time test") {
    StatTape tape(writeExample, 1, 10, 100);
    for (int i = 0; i < 1000; ++i) {
        tape.write(i); // (rw + move) * 1000 == 11000
    }
    tape.flush();
    tape.move(-1000); // reset == 100
    CHECK(tape.read() == 0); //rw + move == 11
    tape.move(3); // 3 * move == 30
    CHECK(tape.read() == 4); // rw + move == 11
    CHECK(tape.getTime() == 11152);
    tape.resetTimer();
    CHECK(tape.getTime() == 0);
}

TEST_CASE("Simple sort big memory test") {
    SUBCASE("Small") {
        checkSorter(smallExample, 3, true);
    }
    SUBCASE("Zero") {
        checkSorter(zeroExample, 1, true);
    }
    SUBCASE("One") {
        checkSorter(oneExample, 1, true);
    }
    SUBCASE("Large") {
        checkSorter(largeExample, 10000, true);
    }
}

TEST_CASE("Simple sort small memory test") {
    SUBCASE("Small") {
        checkSorter(smallExample, 1, true);
    }
    SUBCASE("Large 100") {
        checkSorter(largeExample, 100, true);
    }
    SUBCASE("Large 1") {
        checkSorter(largeExample, 1, true);
    }
}

TEST_CASE("Fast sort big memory test") {
    SUBCASE("Small") {
        checkSorter(smallExample, 3, false);
    }
    SUBCASE("Zero") {
        checkSorter(zeroExample, 1, false);
    }
    SUBCASE("One") {
        checkSorter(oneExample, 1, false);
    }
    SUBCASE("Large") {
        checkSorter(largeExample, 10000, false);
    }
}

TEST_CASE("Fast sort small memory test") {
    SUBCASE("Small") {
        checkSorter(smallExample, 1, false);
    }
    SUBCASE("Large 100") {
        checkSorter(largeExample, 100, false);
    }
    SUBCASE("Large 1") {
        checkSorter(largeExample, 1, false);
    }
}

TEST_CASE("Not enough tapes") {
    SUBCASE("Simple") {
        FileTape inFile(oneExample);
        FileTape outFile(writeExample);

        std::vector<Tape*> tapes;
        SimpleSorter sorter(10);
        CHECK_THROWS(sorter.sort(&inFile, &outFile, tapes));
    }

    SUBCASE("Fast") {
        FileTape inFile(oneExample);
        FileTape outFile(writeExample);
        FileTape tmp1File(tmp1Example);
        FileTape tmp2File(tmp2Example);

        std::vector<Tape*> tapes = {&tmp1File, &tmp2File};
        FastSorter sorter(10);
        CHECK_THROWS(sorter.sort(&inFile, &outFile, tapes));
    }
}