/*
 * =====================================================================================
 *
 *       Filename:  octree.h
 *
 *    Description:  Octree impl
 *
 *        Version:  1.0
 *        Created:  2014/09/12 18时25分43秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (http://xcoder.in/), admin@xcoder.in
 *   Organization:  Touhou Gensokyo
 *
 * =====================================================================================
 */
#ifndef __OCTREE_H__
#define __OCTREE_H__

#include <v8.h>
#include "common.h"
#include <list>
#include <vector>
using namespace thmclrx;

struct OctreeNode
{
    int red;
    int green;
    int blue;

    bool isLeaf;
    int pixelCount;

    OctreeNode* children[8];
};

class Octree
{
private:
    OctreeNode* root;
    list<OctreeNode*> reducible[7];
    int leafCount;

public:
    Octree();
    ~Octree();
    void buildTree(RGB* pixels[], int pixelCount, int maxColor);
    void addColor(OctreeNode* node, RGB* color, int level);
    void reduceTree();
    void colorStats(OctreeNode* node, vector<ColorCount*>* colors);
    OctreeNode* getRoot() { return root; }

    static void recycleColorCount(vector<ColorCount*>* colors);
    static void cleanPool();
};

#endif

