/*
 * =====================================================================================
 *
 *       Filename:  mindiffer.h
 *
 *    Description:  Min differ
 *
 *        Version:  1.0
 *        Created:  2014/09/15 19时33分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (http://xcoder.in/), admin@xcoder.in
 *   Organization:  Touhou Gensokyo
 *
 * =====================================================================================
 */
#ifndef __MINDIFFER_H__
#define __MINDIFFER_H__

#include "common.h"
#include <v8.h>
#include <list>
#include <vector>
using namespace std;
using namespace v8;

const Handle<String> symbolR = String::New("r");
const Handle<String> symbolG = String::New("g");
const Handle<String> symbolB = String::New("b");

struct RGBWithCount
{
    int count;
    thmclrx::RGB* rgb;
};

class MinDiffer
{
private:
    vector<RGBWithCount>* _pixels;
    vector<thmclrx::Palette>* _palette;

public:
    static bool TransformColorParam(Local<Value> param, vector<RGBWithCount>* rgb);

    MinDiffer(vector<RGBWithCount>* pixels, vector<thmclrx::Palette>* palette);
    void calculate(vector<thmclrx::ColorCount*>* colors);
};

#endif

