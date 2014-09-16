/*
 * =====================================================================================
 *
 *       Filename:  thmclrx.cpp
 *
 *    Description:  Theme Color eXtractor
 *
 *        Version:  1.0
 *        Created:  2014/09/12 17时30分29秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (http://xcoder.in/), admin@xcoder.in
 *   Organization:  Touhou Gensokyo
 *
 * =====================================================================================
 */
#include <node.h>
#include <v8.h>
#include <vector>
#include "octree.h"
#include "mindiffer.h"
using namespace std;

Handle<Value> MindifferGet(const Arguments& args)
{
    HandleScope scope;

    if(args.Length() < 1)
    {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments.")));
        return scope.Close(Undefined());
    }

    Local<Value> _rgbArray = args[0];
    vector<RGBWithCount> rgbArray;

    // tranform color param
    if(!MinDiffer::TransformColorParam(_rgbArray, &rgbArray))
    {
        for(int i = 0; i < rgbArray.size(); i++)
        {
            g_PoolRGB.Recycle(rgbArray[i].rgb);
        }

        rgbArray.clear();

        return scope.Close(Undefined());
    }

    // color palette
    vector<Palette> palette;
    if(args.Length() == 1) Palette::GetDefaultPalette(&palette);
    else
    {
        Local<Value> _palette = args[1];
        Palette::V8ToPalette(_palette, &palette);
    }

    MinDiffer mindiffer(&rgbArray, &palette);

    // calculate...
    vector<ColorCount*> colorCount;
    mindiffer.calculate(&colorCount);

    // translate vector to v8::Array
    Local<Array> result = Array::New(colorCount.size());
    for(int i = 0; i < colorCount.size(); i++)
    {
        Local<Object> obj = Object::New();
        obj->Set(String::NewSymbol("color"), String::NewSymbol(colorCount[i]->color));
        obj->Set(String::NewSymbol("count"), Integer::New(colorCount[i]->count));

        result->Set(i, obj);
    }

    // recycle something
    for(int i = 0; i < rgbArray.size(); i++)
    {
        g_PoolRGB.Recycle(rgbArray[i].rgb);
    }
    rgbArray.clear();
    Octree::recycleColorCount(&colorCount);

    return scope.Close(result);
}

Handle<Value> OctreeGet(const Arguments& args)
{
    HandleScope scope;

    if(args.Length() < 1)
    {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments.")));
        return scope.Close(Undefined());
    }

    // Needs one argument - RGB Object Array
    Local<Value> _rgbArray = args[0];
    if(!_rgbArray->IsArray()) 
    {
        ThrowException(Exception::TypeError(String::New("Argument should be an array.")));
        return scope.Close(Undefined());
    }

    // max colors
    int maxColor = 256;
    if(args.Length() > 1 && args[1]->IsInt32())
    {
        maxColor = args[1]->ToInt32()->Value();
    }

    Local<Array> rgbArray = Local<Array>::Cast(_rgbArray);

    // refer to: http://deadhorse.me/nodejs/2012/10/09/c_addon_in_nodejs_object.html
    int pixelCount = rgbArray->Length();
    thmclrx::RGB* pRGBs[pixelCount];
    for(int i = 0; i < pixelCount; i++)
    {
        Local<Value> v = rgbArray->Get(i);
        if(!v->IsObject())
        {
            // recycle...
            thmclrx::RGB::recycleArray(pRGBs, i - 1);

            ThrowException(Exception::TypeError(String::New("Pixel array is not all objects.")));
            return scope.Close(Undefined());
        }

        Local<Object> preRGB = rgbArray->Get(i)->ToObject();
        if(!preRGB->Get(String::NewSymbol("r"))->IsInt32() ||
                !preRGB->Get(String::NewSymbol("g"))->IsInt32() ||
                !preRGB->Get(String::NewSymbol("b"))->IsInt32())
        {
            thmclrx::RGB::recycleArray(pRGBs, i - 1);
            ThrowException(Exception::TypeError(String::New("Pixel array's elements must contains r, g and b.")));
            return scope.Close(Undefined());
        }

        thmclrx::RGB* tmp = g_PoolRGB.Create();
        tmp->red = preRGB->Get(String::NewSymbol("r"))->ToInt32()->Value();
        tmp->green = preRGB->Get(String::NewSymbol("g"))->ToInt32()->Value();
        tmp->blue = preRGB->Get(String::NewSymbol("b"))->ToInt32()->Value();

        pRGBs[i] = tmp;
    }

    // create an octree
    Octree tree;
    tree.buildTree(pRGBs, pixelCount, maxColor);

    // tree stats
    vector<ColorCount*> colorCount;
    tree.colorStats(tree.getRoot(), &colorCount);

    // translate vector to v8::Array
    Local<Array> result = Array::New(colorCount.size());
    for(int i = 0; i < colorCount.size(); i++)
    {
        Local<Object> obj = Object::New();
        obj->Set(String::NewSymbol("color"), String::NewSymbol(colorCount[i]->color));
        obj->Set(String::NewSymbol("count"), Integer::New(colorCount[i]->count));

        result->Set(i, obj);
    }

    // recycle...
    Octree::recycleColorCount(&colorCount);
    thmclrx::RGB::recycleArray(pRGBs, pixelCount);

    return scope.Close(result);
}

Handle<Value> CleanPool(const Arguments& args)
{
    HandleScope scope;

    g_PoolRGB.Clean();
    g_PoolColorCount.Clean();
    Octree::cleanPool();

    return scope.Close(Undefined());
}

void Init(Handle<Object> exports)
{
    exports->Set(String::NewSymbol("octreeGet"),
            FunctionTemplate::New(OctreeGet)->GetFunction());
    exports->Set(String::NewSymbol("mindifferGet"),
            FunctionTemplate::New(MindifferGet)->GetFunction());
    exports->Set(String::NewSymbol("cleanPool"),
            FunctionTemplate::New(CleanPool)->GetFunction());
}

NODE_MODULE(thmclrx, Init);

