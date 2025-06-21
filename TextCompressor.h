// TextCompressor.h

#ifndef __TEXTCOMPRESSOR_H__
#define __TEXTCOMPRESSOR_H__

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <vector>
#include <string>
#include "HuffTree.h"

class TextCompressor {
public:
    // 统计频率
    static std::unordered_map<char, int> countFrequencies(const std::string &text) {
        std::unordered_map<char, int> freq;
        for (char c : text) freq[c]++;
        return freq;
    }

    // 构建 HuffTree
    static HuffTree<char>* buildTree(const std::unordered_map<char, int> &freq) {
        std::priority_queue<HuffTree<char>*, std::vector<HuffTree<char>*>, minTreeComp<char>> pq;
        for (const auto &pair : freq) {
            pq.push(new HuffTree<char>(pair.first, pair.second));
        }

        while (pq.size() > 1) {
            HuffTree<char> *left = pq.top(); pq.pop();
            HuffTree<char> *right = pq.top(); pq.pop();
            pq.push(new HuffTree<char>(left, right));
        }

        return pq.top();
    }

    // 构建编码表
    static void buildCodeTable(HuffNode<char>* node, std::string code,
                               std::unordered_map<char, std::string> &codeTable) {
        if (!node) return;

        if (node->isLeaf()) {
            auto leaf = dynamic_cast<LeafHuffNode<char>*>(node);
            codeTable[leaf->getData()] = code;
            return;
        }

        auto internal = dynamic_cast<InternalHuffNode<char>*>(node);
        buildCodeTable(internal->getLeft(), code + "0", codeTable);
        buildCodeTable(internal->getRight(), code + "1", codeTable);
    }

    // 编码文本
    static std::string encodeText(const std::string &text,
                                  const std::unordered_map<char, std::string> &codeTable) {
        std::string encoded;
        for (char c : text) encoded += codeTable.at(c);
        return encoded;
    }

    // 写入编码和编码表到文件
    static void saveEncoded(const std::string &binary, const std::unordered_map<char, std::string> &codeTable) {
        std::ofstream outCode("encoded.txt");
        for (auto &pair : codeTable)
            outCode << (int)(unsigned char)pair.first << " " << pair.second << "\n";
        outCode << "===\n";
        outCode << binary;
        outCode.close();
    }

    // 读取编码和编码表
    static void loadEncoded(std::string &binary, std::unordered_map<std::string, char> &decodeTable) {
        std::ifstream in("encoded.txt");
        std::string line;
        while (getline(in, line) && line != "===") {
            int ascii;
            std::string code;
            std::stringstream ss(line);
            ss >> ascii >> code;
            decodeTable[code] = (char)ascii;
        }
        binary = "";
        char c;
        while (in.get(c)) binary += c;
        in.close();
    }

    // 解码文本
    static std::string decodeText(const std::string &binary, const std::unordered_map<std::string, char> &decodeTable) {
        std::string decoded, buffer;
        for (char bit : binary) {
            buffer += bit;
            if (decodeTable.count(buffer)) {
                decoded += decodeTable.at(buffer);
                buffer = "";
            }
        }
        return decoded;
    }
};

#endif
