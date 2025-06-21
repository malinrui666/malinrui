// main.cpp

#include "ArrayHuffTree.h"

int main() {
    std::vector<std::pair<char, int>> charFreqs = {
        {'a', 45},
        {'b', 13},
        {'c', 12},
        {'d', 16},
        {'e', 9},
        {'f', 5}
    };

    ArrayHuffTree tree(charFreqs);
    tree.printCodes();

    return 0;
}
