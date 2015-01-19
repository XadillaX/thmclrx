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
#include <nan.h>
#include <vector>
#include "octree.h"
#include "mindiffer.h"
using namespace std;
using namespace v8;

NAN_METHOD(MindifferGet)
{
    NanScope();

    if(args.Length() < 1)
    {
        NanThrowError("Wrong number of arguments.");
        NanReturnUndefined();
    }

    Local<Value> _rgbArray = args[0];
    vector<RGBWithCount> rgbArray;

    // tranform color param
    if(!MinDiffer::TransformColorParam(_rgbArray, &rgbArray))
    {
        for(unsigned int i = 0; i < rgbArray.size(); i++)
        {
            g_PoolRGB.Recycle(rgbArray[i].rgb);
        }

        rgbArray.clear();

        NanReturnUndefined();
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
    Local<Array> result = NanNew<Array>(colorCount.size());
    for(unsigned int i = 0; i < colorCount.size(); i++)
    {
        Local<Object> obj = NanNew<Object>();
        obj->Set(NanNew<String>("color"), NanNew<String>(colorCount[i]->color));
        obj->Set(NanNew<String>("count"), NanNew<Integer>(colorCount[i]->count));
        result->Set(i, obj);
    }

    // recycle something
    for(unsigned int i = 0; i < rgbArray.size(); i++)
    {
        g_PoolRGB.Recycle(rgbArray[i].rgb);
    }
    rgbArray.clear();
    Octree::recycleColorCount(&colorCount);

    NanReturnValue(result);
}

NAN_METHOD(OctreeGet)
{
    NanScope();

    if(args.Length() < 1)
    {
        NanThrowError("Wrong number of arguments.");
        NanReturnUndefined();
    }

    // Needs one argument - RGB Object Array
    Local<Value> _rgbArray = args[0];
    if(!_rgbArray->IsArray()) 
    {
        NanThrowTypeError("Arguments should be an array.");
        NanReturnUndefined();
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
    thmclrx::RGB** pRGBs = new thmclrx::RGB*[pixelCount]; //[pixelCount];
    for(int i = 0; i < pixelCount; i++)
    {
       Local<Value> v = rgbArray->Get(i);
        if(!v->IsObject())
        {
            // recycle...
            thmclrx::RGB::recycleArray(pRGBs, i - 1);

            NanThrowTypeError("Pixel array is not all objects.");
            NanReturnUndefined();
        }

        Local<Object> preRGB = rgbArray->Get(i)->ToObject();
        if(!preRGB->Get(NanNew<String>("r"))->IsInt32() ||
                !preRGB->Get(NanNew<String>("g"))->IsInt32() ||
                !preRGB->Get(NanNew<String>("b"))->IsInt32())
        {
            thmclrx::RGB::recycleArray(pRGBs, i - 1);
            NanThrowTypeError("Pixel array's elements must contains r, g and b.");
            NanReturnUndefined();
        }

        thmclrx::RGB* tmp = g_PoolRGB.Create();
        tmp->red = preRGB->Get(NanNew<String>("r"))->ToInt32()->Value();
        tmp->green = preRGB->Get(NanNew<String>("g"))->ToInt32()->Value();
        tmp->blue = preRGB->Get(NanNew<String>("b"))->ToInt32()->Value();

        pRGBs[i] = tmp;
    }

    // create an octree
    Octree tree;
    tree.buildTree(pRGBs, pixelCount, maxColor);

    // tree stats
    vector<ColorCount*> colorCount;
    tree.colorStats(tree.getRoot(), &colorCount);

    // translate vector to v8::Array
    Local<Array> result = NanNew<Array>(colorCount.size());
    Local<String> colorSymbol = NanNew<String>("color");
    Local<String> countSymbol = NanNew<String>("count");
    for(unsigned int i = 0; i < colorCount.size(); i++)
    {
        Local<Object> obj = NanNew<Object>();
        obj->Set(colorSymbol, NanNew<String>(colorCount[i]->color));
        obj->Set(countSymbol, NanNew<Integer>(colorCount[i]->count));

        result->Set(i, obj);
    }

    // recycle...
    Octree::recycleColorCount(&colorCount);
    thmclrx::RGB::recycleArray(pRGBs, pixelCount);
    delete []pRGBs;

    NanReturnValue(result);
}

NAN_METHOD(CleanPool)
{
    NanScope();

    g_PoolRGB.Clean();
    g_PoolColorCount.Clean();
    Octree::cleanPool();

    NanReturnUndefined();
}

void Init(Handle<Object> exports)
{
    exports->Set(NanNew<String>("octreeGet"),
            NanNew<FunctionTemplate>(OctreeGet)->GetFunction());
    exports->Set(NanNew<String>("mindifferGet"),
            NanNew<FunctionTemplate>(MindifferGet)->GetFunction());
    exports->Set(NanNew<String>("cleanPool"),
            NanNew<FunctionTemplate>(CleanPool)->GetFunction());
}

NODE_MODULE(thmclrx, Init);

