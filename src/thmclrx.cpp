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
#include <nan.h>
#include <vector>
#include "octree.h"
#include "mindiffer.h"
using namespace std;

using Nan::Set;
using Nan::GetFunction;
using Nan::New;
using v8::FunctionTemplate;
using v8::String;

NAN_METHOD(MindifferGet)
{
    if(info.Length() < 1)
    {
        return Nan::ThrowError("Wrong number of arguments.");
    }

    Local<Value> _rgbArray = info[0];
    vector<RGBWithCount> rgbArray;

    // tranform color param
    if(!MinDiffer::TransformColorParam(_rgbArray, &rgbArray))
    {
        for(unsigned int i = 0; i < rgbArray.size(); i++)
        {
            g_PoolRGB.Recycle(rgbArray[i].rgb);
        }

        rgbArray.clear();

        info.GetReturnValue().Set(Nan::Undefined());
        return;
    }

    // color palette
    vector<Palette> palette;
    if(info.Length() == 1) Palette::GetDefaultPalette(&palette);
    else
    {
        Local<Value> _palette = info[1];
        Palette::V8ToPalette(_palette, &palette);
    }

    MinDiffer mindiffer(&rgbArray, &palette);

    // calculate...
    vector<ColorCount*> colorCount;
    mindiffer.calculate(&colorCount);

    // translate vector to v8::Array
    Local<Array> result = Nan::New<Array>(colorCount.size());
    for(unsigned int i = 0; i < colorCount.size(); i++)
    {
        Local<Object> obj = Nan::New<Object>();
        obj->Set(Nan::New<String>("color").ToLocalChecked(),
                Nan::New<String>(colorCount[i]->color).ToLocalChecked());

        obj->Set(Nan::New<String>("count").ToLocalChecked(),
                Nan::New<Integer>(colorCount[i]->count));

        result->Set(i, obj);
    }

    // recycle something
    for(unsigned int i = 0; i < rgbArray.size(); i++)
    {
        g_PoolRGB.Recycle(rgbArray[i].rgb);
    }
    rgbArray.clear();
    Octree::recycleColorCount(&colorCount);

    info.GetReturnValue().Set(result);
}

NAN_METHOD(OctreeGet)
{
    if(info.Length() < 1)
    {
        return Nan::ThrowError("Wrong number of arguments.");
    }

    // Needs one argument - RGB Object Array
    Local<Value> _rgbArray = info[0];
    if(!_rgbArray->IsArray()) 
    {
        Nan::ThrowTypeError("Arguments should be an array.");
        info.GetReturnValue().Set(Nan::Undefined());
        return;
    }

    // max colors
    int maxColor = 256;
    if(info.Length() > 1 && info[1]->IsInt32())
    {
        maxColor = info[1]->ToInt32()->Value();
    }

    Local<Array> rgbArray = Local<Array>::Cast(_rgbArray);

    // refer to: http://deadhorse.me/nodejs/2012/10/09/c_addon_in_nodejs_object.html
    int pixelCount = rgbArray->Length();
    thmclrx::RGB** pRGBs = new thmclrx::RGB*[pixelCount]; //[pixelCount];
    for(int i = 0; i < pixelCount; i++)
    {
       Local<Value> v = rgbArray->Get(i);
        if(!v->IsObject())
        {
            // recycle...
            thmclrx::RGB::recycleArray(pRGBs, i - 1);
            return Nan::ThrowTypeError("Pixel array is not all objects.");
        }

        Local<Object> preRGB = rgbArray->Get(i)->ToObject();
        if(!preRGB->Get(Nan::New<String>("r").ToLocalChecked())->IsInt32() ||
                !preRGB->Get(Nan::New<String>("g").ToLocalChecked())->IsInt32() ||
                !preRGB->Get(Nan::New<String>("b").ToLocalChecked())->IsInt32())
        {
            thmclrx::RGB::recycleArray(pRGBs, i - 1);
            return Nan::ThrowTypeError("Pixel array's elements must contains r, g and b.");
        }

        thmclrx::RGB* tmp = g_PoolRGB.Create();
        tmp->red = preRGB->Get(Nan::New<String>("r").ToLocalChecked())->ToInt32()->Value();
        tmp->green = preRGB->Get(Nan::New<String>("g").ToLocalChecked())->ToInt32()->Value();
        tmp->blue = preRGB->Get(Nan::New<String>("b").ToLocalChecked())->ToInt32()->Value();

        pRGBs[i] = tmp;
    }

    // create an octree
    Octree tree;
    tree.buildTree(pRGBs, pixelCount, maxColor);

    // tree stats
    vector<ColorCount*> colorCount;
    tree.colorStats(tree.getRoot(), &colorCount);

    // translate vector to v8::Array
    Local<Array> result = Nan::New<Array>(colorCount.size());
    Local<String> colorSymbol = Nan::New<String>("color").ToLocalChecked();
    Local<String> countSymbol = Nan::New<String>("count").ToLocalChecked();
    for(unsigned int i = 0; i < colorCount.size(); i++)
    {
        Local<Object> obj = Nan::New<Object>();
        obj->Set(colorSymbol, Nan::New<String>(colorCount[i]->color).ToLocalChecked());
        obj->Set(countSymbol, Nan::New<Integer>(colorCount[i]->count));

        result->Set(i, obj);
    }

    // recycle...
    Octree::recycleColorCount(&colorCount);
    thmclrx::RGB::recycleArray(pRGBs, pixelCount);
    delete []pRGBs;

    info.GetReturnValue().Set(result);
}

NAN_METHOD(CleanPool)
{
    g_PoolRGB.Clean();
    g_PoolColorCount.Clean();
    Octree::cleanPool();

    info.GetReturnValue().Set(Nan::Undefined());
}

NAN_MODULE_INIT(Init)
{
    Nan::Set(target, Nan::New<String>("octreeGet").ToLocalChecked(),
            Nan::GetFunction(Nan::New<FunctionTemplate>(OctreeGet)).ToLocalChecked());
    Nan::Set(target, Nan::New<String>("mindifferGet").ToLocalChecked(),
            Nan::GetFunction(Nan::New<FunctionTemplate>(MindifferGet)).ToLocalChecked());
    Nan::Set(target, Nan::New<String>("cleanPool").ToLocalChecked(),
            Nan::GetFunction(Nan::New<FunctionTemplate>(CleanPool)).ToLocalChecked());
}

NODE_MODULE(thmclrx, Init);
