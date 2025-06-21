// ImageCompressor.h

#ifndef __IMAGECOMPRESSOR_H__
#define __IMAGECOMPRESSOR_H__

#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include "HuffTree.h"

class ImageCompressor {
public:
    // 读取图像为字节数组
    static std::vector<unsigned char> readBinaryFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::binary);
        return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

    // 写入字节数组为文件
    static void writeBinaryFile(const std::string &filename, const std::vector<unsigned char> &data) {
        std::ofstream file(filename, std::ios::binary);
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }

    // 压缩
    static void compress(const std::string &inFile, const std::string &outFile) {
        auto bytes = readBinaryFile(inFile);

        std::unordered_map<unsigned char, int> freq;
        for (auto b : bytes) freq[b]++;

        std::priority_queue<HuffTree<unsigned char>*, std::vector<HuffTree<unsigned char>*>, minTreeComp<unsigned char>> pq;
        for (auto &pair : freq)
            pq.push(new HuffTree<unsigned char>(pair.first, pair.second));

        while (pq.size() > 1) {
            auto l = pq.top(); pq.pop();
            auto r = pq.top(); pq.pop();
            pq.push(new HuffTree<unsigned char>(l, r));
        }

        auto tree = pq.top();

        std::unordered_map<unsigned char, std::string> codeTable;
        buildCodeTable(tree->getRoot(), "", codeTable);

        std::string encoded;
        for (auto b : bytes) encoded += codeTable[b];

        // 保存编码和表
        std::ofstream out(outFile);
        for (auto &pair : codeTable)
            out << (int)(unsigned char)pair.first << " " << pair.second << "\n";
        out << "===\n";
        out << encoded;
        out.close();
    }

    // 解压
    static void decompress(const std::string &encodedFile, const std::string &outFile) {
        std::ifstream in(encodedFile);
        std::unordered_map<std::string, unsigned char> decodeTable;
        std::string line;

        while (getline(in, line) && line != "===") {
            int byteVal;
            std::string code;
            std::stringstream ss(line);
            ss >> byteVal >> code;
            decodeTable[code] = static_cast<unsigned char>(byteVal);
        }

        std::string binary;
        char c;
        while (in.get(c)) binary += c;
        in.close();

        std::vector<unsigned char> result;
        std::string buffer;
        for (char bit : binary) {
            buffer += bit;
            if (decodeTable.count(buffer)) {
                result.push_back(decodeTable[buffer]);
                buffer = "";
            }
        }

        writeBinaryFile(outFile, result);
    }

private:
    static void buildCodeTable(HuffNode<unsigned char>* node, std::string code,
                               std::unordered_map<unsigned char, std::string> &table) {
        if (!node) return;
        if (node->isLeaf()) {
            auto leaf = dynamic_cast<LeafHuffNode<unsigned char>*>(node);
            table[leaf->getData()] = code;
            return;
        }
        auto internal = dynamic_cast<InternalHuffNode<unsigned char>*>(node);
        buildCodeTable(internal->getLeft(), code + "0", table);
        buildCodeTable(internal->getRight(), code + "1", table);
    }
};

#endif
