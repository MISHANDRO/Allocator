#include "Test.h"
#include "MemoryPoolsAllocator.h"

#include <ctime>
#include <list>
#include <vector>

Test::Test(const std::filesystem::path& path) {
    if (!exists(path)){
        throw std::invalid_argument("no such folder: " + path.string());
    }

    file_ = std::ofstream(path.string() + "/result.txt");
}

Test::~Test() {
    file_.close();
}

void Test::VectorTest(size_t size) {
    Write("Vector Test\n");

    ASettings::Init(2, PoolSettings(2, size * 2),
                    {PoolSettings(size * 2, 2)});

    std::vector<size_t, MemoryPoolsAllocator<size_t>> obj1;

    begin_time_ = clock();

    for (size_t i = 0; i < size; i++) {
        obj1.push_back(i);
        if (i % 100000 == 0) {
            WriteTime();
        }
    }

    obj1.clear();
    Write("\n");

    std::vector<size_t> obj2;
    begin_time_ = clock();

    for (size_t i = 0; i < size; i++) {
        obj2.push_back(i);
        if (i % 100000 == 0) {
            WriteTime();
        }
    }
}

void Test::ListTest(size_t size) {
    Write("List Test\n");

    ASettings::Init(1, PoolSettings(size, 1));
    std::list<size_t, MemoryPoolsAllocator<size_t>> obj1;

    begin_time_ = clock();

    for (size_t i = 0; i < size; i++) {
        obj1.push_front(i);
        if (i % 100000 == 0) {
            WriteTime();
        }
    }

    obj1.clear();
    Write("\n");

    std::list<size_t> obj2;
    begin_time_ = clock();

    for (size_t i = 0; i < size; i++) {
        obj2.push_front(i);
        if (i % 100000 == 0) {
            WriteTime();
        }
    }
}

void Test::Write(const std::string& msg) {
    file_ << msg;
}

void Test::WriteTime() {
    file_ << (clock() - begin_time_) << " ";
}
