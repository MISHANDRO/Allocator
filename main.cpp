#include "Test.h"

int main(int argc, const char** argv) {

    if (argc == 4) {
        Test test(argv[3]);

        if (std::string(argv[1]) == "vector") {
            test.VectorTest(std::strtol(argv[2], nullptr, 10));
        } else if (std::string(argv[1]) == "list") {
            test.ListTest(std::strtol(argv[2], nullptr, 10));
        }
    }

    return 0;
}
