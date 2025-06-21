#include <iostream>
#include "HuffTree.h"

int main() {
    char letters[] = {'a', 'b', 'c', 'd', 'e', 'f'};

    HuffTree<char>* Forest[6];
    Forest[0] = new HuffTree<char>(letters[0], 45);
    Forest[1] = new HuffTree<char>(letters[1], 13);
    Forest[2] = new HuffTree<char>(letters[2], 12);
    Forest[3] = new HuffTree<char>(letters[3], 16);
    Forest[4] = new HuffTree<char>(letters[4], 9);
    Forest[5] = new HuffTree<char>(letters[5], 5);

    HuffTree<char>* huff = buildHuff(Forest, 6);
    huff->print();

    return 0;
}

#CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HuffmanTree)

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp)
