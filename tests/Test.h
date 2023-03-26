#pragma once

#include "Allocator.h"

#include "CirtucalBufferExt.h"

#include <ctime>
#include <list>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <set>
#include <queue>

class Test {
public:
    explicit Test(const std::filesystem::path& path) {
        if (!exists(path)){
            throw std::invalid_argument("no such folder: " + path.string());
        }

        file_ = std::ofstream(path.string() + "/result.txt");
    }

    ~Test() {
        file_.close();
    }

    void VectorTest(size_t size) {
        Write("Vector Test\n");

//        ASettings::Init(3, PoolSettings{100, size / 100},
//                        {PoolSettings{size / 100, 8},PoolSettings{size / 1000, 256}});

        ASettings::Init(2, PoolSettings{2, size * 2},
                        {PoolSettings{size * 2, 2}});

        std::vector<size_t, Allocator<size_t>> obj1;

        begin_time_ = clock();

        for (size_t i = 0; i < size; i++) {
            obj1.push_back(i);
            if (i % 1000000 == 0) {
                WriteTime();
            }
        }

        obj1.clear();
        Write("\n");

        std::vector<size_t> obj2;
        begin_time_ = clock();

        for (size_t i = 0; i < size; i++) {
            obj2.push_back(i);
            if (i % 1000000 == 0) {
                WriteTime();
            }
        }
    }

    void ListTest(size_t size) {
        Write("List Test\n");

        ASettings::Init(1, PoolSettings(size, 1));
        std::list<size_t, Allocator<size_t>> obj1;

        begin_time_ = clock();

        for (size_t i = 0; i < size; i++) {
            obj1.push_front(i);
            if (i % 1000000 == 0) {
                WriteTime();
            }
        }

        obj1.clear();
        Write("\n");

        std::list<size_t> obj2;
        begin_time_ = clock();

        for (size_t i = 0; i < size; i++) {
            obj2.push_front(i);
            if (i % 1000000 == 0) {
                WriteTime();
            }
        }
    }

private:
    void Write(const std::string& msg) {
        file_ << msg;
    }

    void WriteTime() {
        std::string msg = std::to_string(clock() - begin_time_);
        file_ << msg << " ";
    }

    clock_t begin_time_ = 0;
    std::ofstream file_;
};
