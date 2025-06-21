// HuffNode.h

#ifndef __HUFFMANNODE_H__
#define __HUFFMANNODE_H__

template <typename Data>
class HuffNode {
public:
    virtual int getWeight() = 0;
    virtual bool isLeaf() = 0;
};

template <typename Data>
class LeafHuffNode : public HuffNode<Data> {
private:
    Data data;
    int weight;

public:
    LeafHuffNode(const Data &val, int freq) : data(val), weight(freq) {}

    int getWeight() override { return weight; }

    bool isLeaf() override { return true; }

    Data getData() { return data; }
};

template <typename Data>
class InternalHuffNode : public HuffNode<Data> {
private:
    HuffNode<Data> *leftChild;
    HuffNode<Data> *rightChild;
    int weight;

public:
    InternalHuffNode(HuffNode<Data> *l, HuffNode<Data> *r)
        : leftChild(l), rightChild(r), weight(l->getWeight() + r->getWeight()) {}

    int getWeight() override { return weight; }

    bool isLeaf() override { return false; }

    HuffNode<Data> *getLeft() const { return leftChild; }
    HuffNode<Data> *getRight() const { return rightChild; }
    void setLeft(HuffNode<Data> *l) { leftChild = l; }
    void setRight(HuffNode<Data> *r) { rightChild = r; }
};

#endif
