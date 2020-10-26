#include <algorithm>
#include <Imagine/Graphics.h>
#include "quadtree/quadtree.h"
using namespace Imagine;
using namespace std;

QuadTree<byte> *black = new QuadLeaf<byte>(byte(0));    // black leaf pointer (global variable)
QuadTree<byte> *white = new QuadLeaf<byte>(byte(255));  // white leaf pointer (global variable)

template<typename T>
QuadTree<T>* dfs(T* image, int x, int y, int length, int const lengthOriginal) {
    if (length == 1) {
        return new QuadLeaf<T>(image[lengthOriginal * y + x]);
    }
    length /= 2;
    auto northWest = dfs(image, x, y, length, lengthOriginal);
    auto northEast = dfs(image, x + length, y, length, lengthOriginal);
    auto southEast = dfs(image, x + length, y + length, length, lengthOriginal);
    auto southWest = dfs(image, x, y + length, length, lengthOriginal);
    
    if (sameLeaves(northWest, northEast, southEast, southWest)) {
        T val = northWest->value();
        delete northWest;
        delete northEast;
        delete southEast;
        delete southWest;
        return new QuadLeaf<T>(val);
    }
    else {
        return new QuadNode<T>(northWest, northEast, southEast, southWest);
    }
};

template<typename T>
QuadTree<T>* dfs(T* image, int x, int y, int length, int const width, int const height, bool const fillByWhite) {
    if (length == 1) {
        if (x < width && y < height) {
            return new QuadLeaf<T>(image[width * y + x]);
        }
        else {
            if (fillByWhite) {
                return new QuadLeaf<T>(byte(255));
            }
            else {
                return new QuadLeaf<T>(byte(0));
            }
        }
    }
    length /= 2;
    auto northWest = dfs(image, x, y, length, width, height, fillByWhite);
    auto northEast = dfs(image, x + length, y, length, width, height, fillByWhite);
    auto southEast = dfs(image, x + length, y + length, length, width, height, fillByWhite);
    auto southWest = dfs(image, x, y + length, length, width, height, fillByWhite);
    
    if (sameLeaves(northWest, northEast, southEast, southWest)) {
        T val = northWest->value();
        delete northWest;
        delete northEast;
        delete southEast;
        delete southWest;
        return new QuadLeaf<T>(val);
    }
    else {
        return new QuadNode<T>(northWest, northEast, southEast, southWest);
    }
};

template<typename T>
T thresholdAccordingToLength(int length, int lengthMax, T thresholdMin) {
    int factor = log2(lengthMax / length);
    return T(thresholdMin * (log2(factor) + 1));
};

template<typename T>
QuadTree<T>* dfsLossyCompression(T* image, int x, int y, int length, int const width, int const height, bool const fillByWhite, T const thresholdMin) {
    if (length == 1) {
        if (x < width && y < height) {
            return new QuadLeaf<T>(image[width * y + x]);
        }
        else {
            if (fillByWhite) {
                return new QuadLeaf<T>(byte(255));
            }
            else {
                return new QuadLeaf<T>(byte(0));
            }
        }
    }
    length /= 2;
    
    auto northWest = dfsLossyCompression(image, x, y, length, width, height, fillByWhite, thresholdMin);
    auto northEast = dfsLossyCompression(image, x + length, y, length, width, height, fillByWhite, thresholdMin);
    auto southEast = dfsLossyCompression(image, x + length, y + length, length, width, height, fillByWhite, thresholdMin);
    auto southWest = dfsLossyCompression(image, x, y + length, length, width, height, fillByWhite, thresholdMin);
    
    T threshold = thresholdAccordingToLength(length, max(width, height), thresholdMin);
    if (similarLeaves(northWest, northEast, southEast, southWest, threshold)) {
        T val = T((northWest->value() + northEast->value() + southEast->value() + southWest->value()) / 4);
        delete northWest;
        delete northEast;
        delete southEast;
        delete southWest;
        return new QuadLeaf<T>(val);
    }
    else {
        return new QuadNode<T>(northWest, northEast, southEast, southWest);
    }
};

template<typename T>
void dfsLossyCompressionRGB(T* imageR, T* imageG, T* imageB, QuadTree<T>*& qR, QuadTree<T>*& qG, QuadTree<T>*& qB, int x, int y, int length, int const width, int const height, bool const fillByWhite, T const thresholdMin) {
    qR = dfsLossyCompression(imageR, x, y, length, width, height, fillByWhite, thresholdMin);
    qG = dfsLossyCompression(imageG, x, y, length, width, height, fillByWhite, thresholdMin);
    qB = dfsLossyCompression(imageB, x, y, length, width, height, fillByWhite, thresholdMin);
};


// for black and white photo
template<typename T>
QuadTree<T>* quaddag(T* image, int x, int y, int length, int const lengthOriginal) {
    if (length == 1) {
        if (image[lengthOriginal * y + x] == black->value()) {
            return black;
        }
        else if (image[lengthOriginal * y + x] == white->value()) {
            return white;
        }
        else {
            throw "Not a black and white image!";
        }
    }
    length /= 2;
    auto northWest = quaddag(image, x, y, length, lengthOriginal);
    auto northEast = quaddag(image, x + length, y, length, lengthOriginal);
    auto southEast = quaddag(image, x + length, y + length, length, lengthOriginal);
    auto southWest = quaddag(image, x, y + length, length, lengthOriginal);
    northWest->protect_leaves_from_destruction = true;
    northEast->protect_leaves_from_destruction = true;
    southEast->protect_leaves_from_destruction = true;
    southWest->protect_leaves_from_destruction = true;
    
    if (sameLeaves(northWest, northEast, southEast, southWest)) {
        T val = northWest->value();
        if (val == black->value()) {
            return black;
        }
        if (val == white->value()) {
            return white;
        }
    }
    else {
        QuadTree<T>* nodeTemp = new QuadNode<T>(northWest, northEast, southEast, southWest);
        nodeTemp->protect_leaves_from_destruction = true;
        return nodeTemp;
    }
};

// quaddag overload for all rectangle images (no matter whether the image is square or not)
template<typename T>
QuadTree<T>* quaddag(T* image, int x, int y, int length, int const width, int const height, bool const fillByWhite) {
    if (length == 1) {
        if (x < width && y < height) {
            if (image[width * y + x] == black->value()) {
                return black;
            }
            else if (image[width * y + x] == white->value()) {
                return white;
            }
            else {
                throw "Not a black and white image!";
            }
        }
        else {
            if (fillByWhite) {
                return white;
            }
            else {
                return black;
            }
        }
    }
    length /= 2;
    auto northWest = quaddag(image, x, y, length, width, height, fillByWhite);
    auto northEast = quaddag(image, x + length, y, length, width, height, fillByWhite);
    auto southEast = quaddag(image, x + length, y + length, length, width, height, fillByWhite);
    auto southWest = quaddag(image, x, y + length, length, width, height, fillByWhite);
    northWest->protect_leaves_from_destruction = true;
    northEast->protect_leaves_from_destruction = true;
    southEast->protect_leaves_from_destruction = true;
    southWest->protect_leaves_from_destruction = true;
    
    if (sameLeaves(northWest, northEast, southEast, southWest)) {
        T val = northWest->value();
        if (val == black->value()) {
            return black;
        }
        if (val == white->value()) {
            return white;
        }
    }
    else {
        QuadTree<T>* nodeTemp = new QuadNode<T>(northWest, northEast, southEast, southWest);
        nodeTemp->protect_leaves_from_destruction = true;
        return nodeTemp;
    }
};

template<typename T>
void quadtreeDecoding(QuadTree<T>* q, T* image, int x, int y, int length, int const lengthOriginal) {
    if (q->isLeaf()) {
        for (int j=0; j < length; j++) {
            for (int i=0; i < length; i++) {
                image[lengthOriginal * (y + j) + (x + i)] = q->value();
            }
        }
    }
    else {
        length /= 2;
        quadtreeDecoding(q->son(0), image, x, y, length, lengthOriginal);
        quadtreeDecoding(q->son(1), image, x + length, y, length, lengthOriginal);
        quadtreeDecoding(q->son(2), image, x + length, y + length, length, lengthOriginal);
        quadtreeDecoding(q->son(3), image, x, y + length, length, lengthOriginal);
    }
};

int nextPow2(int num) {
    int rval = 1;
    while (rval < num) rval <<= 1;
    return rval;
};

template<typename T>
int sizeQuadTree(QuadTree<T>* q, bool const isBlackWhiteDAG) {
    int nbNodes = q->nNodes();
    int treeSize;
    if (isBlackWhiteDAG) {
        treeSize = 2 * sizeof(QuadLeaf<T>) + nbNodes * sizeof(QuadNode<T>);
    }
    else {
        int nbLeaves = q->nLeaves();
        treeSize = nbLeaves * sizeof(QuadLeaf<T>) + nbNodes * sizeof(QuadNode<T>);
    }
    return treeSize;
};

template<typename T>
double imageCompressionRatio(QuadTree<T>* q, int const width, int const height) {
    int nbLeaves = q->nLeaves();
    double compressionRatio = double(nbLeaves) / double(width * height);
    return compressionRatio;
};

template<typename T>
bool sameLeaves(QuadTree<T>* q0, QuadTree<T>* q1, QuadTree<T>* q2, QuadTree<T>* q3) {
    bool res = q0->isLeaf() && q1->isLeaf() && q2->isLeaf() && q3->isLeaf();
    if (res) {
        res = (q0->value() == q1->value()) && (q0->value() == q2->value()) && (q0->value() == q3->value());
    }
    return res;
};

template<typename T>
bool similarLeaves(QuadTree<T>* son0, QuadTree<T>* son1, QuadTree<T>* son2, QuadTree<T>* son3, T threshold) {
    bool res = son0->isLeaf() && son1->isLeaf() && son2->isLeaf() && son3->isLeaf();
    if (res) {
        T maxVal = max(max(son0->value(), son1->value()), max(son2->value(), son3->value()));
        T minVal = min(min(son0->value(), son1->value()), min(son2->value(), son3->value()));
        res = ((maxVal - minVal) < threshold);
    }
    return res;
};
