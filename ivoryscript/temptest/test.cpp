#include <iostream>
#include <ratio>

int main() {
    for (int i = 0; i < 5; ++i) {
        std::cout << (sizeof("") * 8) << std::endl;
    }

    return 0;
}