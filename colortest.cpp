#include <cstddef>
#include <iostream>
#include <sstream>

// program for check the color support in terminal/console
int main(int argc, char *argv[]) {
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            for (size_t k = 0; k < 8; k++) {
                std::stringstream ss;
                ss << i << ";" << (30 + j) << ";" << (40 + k) << "m";
                std::cout << "\x1b[" << ss.str() << "  " << ss.str()
                          << " \x1b[0m";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    return 0;
}
