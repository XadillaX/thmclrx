/*
 * =====================================================================================
 *
 *       Filename:  mindiffer.cpp
 *
 *    Description:  Min differ
 *
 *        Version:  1.0
 *        Created:  2014/09/15 19时34分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (http://xcoder.in/), admin@xcoder.in
 *   Organization:  Touhou Gensokyo
 *
 * =====================================================================================
 */
#include "mindiffer.h"
#include <algorithm>
using namespace std;
using namespace thmclrx;

inline int diff_square(int a, int b)
{
    return (a - b) * (a - b);
}

bool MinDiffer::TransformColorParam(Local<Value> param, vector<RGBWithCount>* rgb)
{
    HandleScope scope;

    Local<String> _ColorParamKeys[] = {
        String::NewSymbol("color"),
        String::NewSymbol("count"),
        String::NewSymbol("r"),
        String::NewSymbol("g"),
        String::NewSymbol("b")
    };

    if(!param->IsArray())
    {
        ThrowException(Exception::TypeError(String::New("The first argument should be an array.")));
        scope.Close(Undefined());
        return false;
    }

    Local<Array> array = Local<Array>::Cast(param->ToObject());
    int length = array->Length();
    char colorStr[7];
    thmclrx::RGB* rgbPixel;
    for(int i = 0; i < length; i++)
    {
        if(!array->Get(i)->IsObject())
        {
            ThrowException(Exception::TypeError(String::New("Elements in first argument must be objects.")));
            scope.Close(Undefined());
            return false;
        }

        Local<Object> obj = array->Get(i)->ToObject();

        if(obj->Get(_ColorParamKeys[0])->IsString() &&
                obj->Get(_ColorParamKeys[1])->IsInt32() &&
                Local<String>::Cast(obj->Get(_ColorParamKeys[0]))->Length() >= 6)
        {
            Local<String>::Cast(obj->Get(_ColorParamKeys[0]))->WriteAscii(colorStr, 0, 6);

            // is all [0123456789ABCDEF]
            for(int j = 0; j < 6; j++)
            {
                if(!(colorStr[j] >= '0' && colorStr[j] <= '9') &&
                        !(colorStr[j] >= 'A' && colorStr[j] <= 'F') &&
                        !(colorStr[j] >= 'a' && colorStr[j] <= 'f'))
                {
                    ThrowException(Exception::TypeError(String::New("Wrong argument in color pixel array 1.")));
                    scope.Close(Undefined());
                    return false;
                }
            }

            // read in
            int c;
            sscanf(colorStr, "%X", &c);

            // R / G / B
            rgbPixel = thmclrx::g_PoolRGB.Create();
            rgbPixel->red = c >> 16;
            rgbPixel->green = (c >> 8) & 0xff;
            rgbPixel->blue = c & 0xff;

            RGBWithCount r = { obj->Get(_ColorParamKeys[1])->ToInt32()->Value(), rgbPixel };

            // push back
            rgb->push_back(r);
        }
        else
        if(obj->Get(_ColorParamKeys[2])->IsInt32() && obj->Get(_ColorParamKeys[3])->IsInt32() && obj->Get(_ColorParamKeys[4])->IsInt32())
        {
            rgbPixel = thmclrx::g_PoolRGB.Create();

            rgbPixel->red = obj->Get(_ColorParamKeys[2])->ToInt32()->Value();
            rgbPixel->green = obj->Get(_ColorParamKeys[3])->ToInt32()->Value();
            rgbPixel->blue = obj->Get(_ColorParamKeys[4])->ToInt32()->Value();

            RGBWithCount r = { 1, rgbPixel };

            rgb->push_back(r);
        }
        else
        {
            ThrowException(Exception::TypeError(String::New("Wrong argument in color pixel array 2.")));
            scope.Close(Undefined());
            return false;
        }
    }

    scope.Close(Undefined());
    return true;
}

MinDiffer::MinDiffer(vector<RGBWithCount>* pixels, vector<Palette>* palette) :
    _pixels(pixels),
    _palette(palette)
{
}

void MinDiffer::calculate(vector<thmclrx::ColorCount*>* colors)
{
    HandleScope scope;

    Local<String> _ColorParamKeys[] = {
        String::NewSymbol("color"),
        String::NewSymbol("count"),
        String::NewSymbol("r"),
        String::NewSymbol("g"),
        String::NewSymbol("b")
    };

    for(int i = 0; i < _palette->size(); i++)
    {
        thmclrx::ColorCount* cnt = thmclrx::g_PoolColorCount.Create();

        cnt->count = 0;
        cnt->red = _palette->at(i).red;
        cnt->green = _palette->at(i).green;
        cnt->blue = _palette->at(i).blue;
        cnt->colorValue = (_palette->at(i).red << 16) + (_palette->at(i).green << 8) + _palette->at(i).blue;
        sprintf(cnt->color, "%.2X%.2X%.2X", cnt->red, cnt->green, cnt->blue);

        colors->push_back(cnt);
    }

    // calculate
    int diffr, diffg, diffb, diff;
    int mindiff, mindiffidx;

    for(int i = 0; i < _pixels->size(); i++)
    {
        mindiff = 0;
        mindiffidx = -1;

        for(int j = 0; j < colors->size(); j++)
        {
            diffr = diff_square(_pixels->at(i).rgb->red, colors->at(j)->red);
            diffg = diff_square(_pixels->at(i).rgb->green, colors->at(j)->green);
            diffb = diff_square(_pixels->at(i).rgb->blue, colors->at(j)->blue);
            diff = diffr + diffg + diffb;

            if(mindiffidx == -1)
            {
                mindiff = diff;
                mindiffidx = j;
                continue;
            }

            if(diff < mindiff)
            {
                mindiff = diff;
                mindiffidx = j;
            }
        }

        if(-1 == mindiffidx) continue;
        colors->at(mindiffidx)->count += _pixels->at(i).count;
    }

    // sort
    sort(colors->begin(), colors->end(), thmclrx::ColorCount::cmp);

    // delete useless ones..
    while(colors->size() && !colors->at(colors->size() - 1)->count)
    {
        colors->pop_back();
    }

    scope.Close(Undefined());
}

