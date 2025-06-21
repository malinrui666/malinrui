#include "TextCompressor.h"
#include <fstream>

int main() {
    // 1. 读取原始文本
    std::ifstream in("test.txt");
    std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    // 2. 编码
    auto freq = TextCompressor::countFrequencies(text);
    HuffTree<char>* tree = TextCompressor::buildTree(freq);

    std::unordered_map<char, std::string> codeTable;
    TextCompressor::buildCodeTable(tree->getRoot(), "", codeTable);

    std::string encoded = TextCompressor::encodeText(text, codeTable);
    TextCompressor::saveEncoded(encoded, codeTable);

    // 3. 解码
    std::unordered_map<std::string, char> decodeTable;
    std::string binary;
    TextCompressor::loadEncoded(binary, decodeTable);
    std::string decoded = TextCompressor::decodeText(binary, decodeTable);

    std::ofstream out("decoded.txt");
    out << decoded;
    out.close();

    return 0;
}
