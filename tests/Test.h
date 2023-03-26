#pragma once

#include <fstream>
#include <filesystem>

class Test {
public:
    explicit Test(const std::filesystem::path& path);
    ~Test();

    void VectorTest(size_t size);
    void ListTest(size_t size);

private:
    void Write(const std::string& msg);
    void WriteTime();

    clock_t begin_time_ = 0;
    std::ofstream file_;
};
