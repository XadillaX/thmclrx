/*
 * =====================================================================================
 *
 *       Filename:  octree.cpp
 *
 *    Description:  Octree impl in Thmclrx
 *
 *        Version:  1.0
 *        Created:  2014/09/12 17时40分57秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (http://xcoder.in/), admin@xcoder.in
 *   Organization:  Touhou Gensokyo
 *
 * =====================================================================================
 */
#include "octree.h"
#include <queue>
#include <string.h>
#include <algorithm>

thmclrx::TCX_MemoryPool<OctreeNode> _octreePool;

void Octree::recycleColorCount(vector<ColorCount*>* colors)
{
    ColorCount* tmp;
    while(colors->size())
    {
        tmp = colors->at(colors->size() - 1);
        g_PoolColorCount.Recycle(tmp);
        colors->pop_back();
    }
}

void Octree::cleanPool()
{
    _octreePool.Clean();
}

Octree::Octree()
{
    root = _octreePool.Create();
    leafCount = 0;
    memset(root, 0, sizeof(OctreeNode));
}

Octree::~Octree()
{
    // recycle octree nodes
    queue<OctreeNode*> q;
    q.push(root);

    // bfs...
    OctreeNode* node;
    while(!q.empty())
    {
        node = q.front();
        q.pop();

        for(int i = 0; i < 8; i++)
        {
            if(NULL == node->children[i]) continue;
            q.push(node->children[i]);
        }

        _octreePool.Recycle(node);
    }
}

void Octree::buildTree(RGB* pixels[], int pixelCount, int maxColor)
{
    for(int i = 0; i < pixelCount; i++)
    {
        addColor(root, pixels[i], 0);
        while(leafCount > maxColor) reduceTree();
    }
}

void Octree::addColor(OctreeNode* node, RGB* color, int level)
{
    if(node->isLeaf)
    {
        node->pixelCount++;
        node->red += color->red;
        node->green += color->green;
        node->blue += color->blue;
    }
    else
    {
        /**
         * eg.
         *   R: 10101101
         *   G: 00101101
         *   B: 10010010
         *
         * idx: 50616616
         */
        unsigned char r = (color->red >> (7 - level)) & 1;
        unsigned char g = (color->green >> (7 - level)) & 1;
        unsigned char b = (color->blue >> (7 - level) & 1);

        int idx = (r << 2) + (g << 1) + b;

        if(NULL == node->children[idx])
        {
            OctreeNode* tmp = node->children[idx] = _octreePool.Create();
            memset(tmp, 0, sizeof(OctreeNode));
            if(level == 7)
            {
                tmp->isLeaf = true;
                leafCount++;
            }
            else
            {
                reducible[level].push_front(tmp);
            }
        }

        addColor(node->children[idx], color, level + 1);
    }
}

void Octree::reduceTree()
{
    // find the deepest level of node
    int lv = 6;
    while(reducible[lv].empty() && lv >= 0) lv--;
    if(lv < 0) return;

    // get the node and remove it from reducible link
    OctreeNode* node = reducible[lv].front();
    reducible[lv].pop_front();

    // merge children
    int r = 0;
    int g = 0;
    int b = 0;
    int count = 0;
    for(int i = 0; i < 8; i++)
    {
        if(NULL == node->children[i]) continue;
        r += node->children[i]->red;
        g += node->children[i]->green;
        b += node->children[i]->blue;
        count += node->children[i]->pixelCount;
        leafCount--;

        // recycle node into pool
        _octreePool.Recycle(node->children[i]);
        node->children[i] = NULL;
    }

    node->isLeaf = true;
    node->red = r;
    node->green = g;
    node->blue = b;
    node->pixelCount = count;
    leafCount++;
}

void Octree::colorStats(OctreeNode* node, vector<ColorCount*>* colors)
{
    if(node->isLeaf)
    {
        int r = node->red / node->pixelCount;
        int g = node->green / node->pixelCount;
        int b = node->blue / node->pixelCount;

        ColorCount* cnt = g_PoolColorCount.Create();
        sprintf(cnt->color, "%.2X%.2X%.2X", r, g, b);
        cnt->count = node->pixelCount;
        cnt->colorValue = (r << 16) + (g << 8) + b;
        cnt->red = r;
        cnt->green = g;
        cnt->blue = b;

        colors->push_back(cnt);

        return;
    }

    for(int i = 0; i < 8; i++)
    {
        if(NULL != node->children[i])
        {
            colorStats(node->children[i], colors);
        }
    }

    // if it's root, sort...
    if(root == node)
    {
        sort(colors->begin(), colors->end(), ColorCount::cmp);
    }
}

