// HuffTree.h

#ifndef __HUFFTREE_H__
#define __HUFFTREE_H__

#include <iostream>
#include <queue>
#include <string>
#include "HuffNode.h"

template <typename Data>
class HuffTree {
private:
    HuffNode<Data> *root;

    void printHelper(HuffNode<Data> *node, const std::string &code) {
        if (node->isLeaf()) {
            LeafHuffNode<Data> *leaf = dynamic_cast<LeafHuffNode<Data> *>(node);
            std::cout << leaf->getData() << " " << code << std::endl;
            return;
        }

        InternalHuffNode<Data> *internal = dynamic_cast<InternalHuffNode<Data> *>(node);
        if (internal->getLeft()) printHelper(internal->getLeft(), code + "0");
        if (internal->getRight()) printHelper(internal->getRight(), code + "1");
    }

public:
    HuffTree(Data val, int freq) {
        root = new LeafHuffNode<Data>(val, freq);
    }

    HuffTree(HuffTree<Data> *l, HuffTree<Data> *r) {
        root = new InternalHuffNode<Data>(l->root, r->root);
    }

    HuffNode<Data> *getRoot() const { return root; }

    int getWeight() const { return root->getWeight(); }

    void print() { printHelper(root, ""); }
};

// comparator for priority queue
template <typename Data>
class minTreeComp {
public:
    bool operator()(const HuffTree<Data> *lhs, const HuffTree<Data> *rhs) const {
        return lhs->getWeight() > rhs->getWeight();
    }
};

// build Huffman tree from forest
template <typename Data>
HuffTree<Data> *buildHuff(HuffTree<Data> *forest[], int size) {
    std::priority_queue<HuffTree<Data> *, std::vector<HuffTree<Data> *>, minTreeComp<Data>> pq;

    for (int i = 0; i < size; ++i) {
        pq.push(forest[i]);
    }

    while (pq.size() > 1) {
        HuffTree<Data> *left = pq.top(); pq.pop();
        HuffTree<Data> *right = pq.top(); pq.pop();
        HuffTree<Data> *parent = new HuffTree<Data>(left, right);
        pq.push(parent);
    }

    return pq.top();
}

#endif
