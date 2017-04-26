/**
 *
 *    ▄▄▄▄▀ ▄  █ █▀▄▀█ ▄█▄    █    █▄▄▄▄     ▄            ,ae,
 * ▀▀▀ █   █   █ █ █ █ █▀ ▀▄  █    █  ▄▀ ▀▄   █          ,88888e
 *     █   ██▀▀█ █ ▄ █ █   ▀  █    █▀▀▌    █ ▀    ,a888b.9888888i
 *    █    █   █ █   █ █▄  ▄▀ ███▄ █  █   ▄ █     888888888888888
 *   ▀        █     █  ▀███▀      ▀  █   █   ▀▄   88888888888888Y
 *           ▀     ▀                ▀     ▀       '8888888888888'
 *                                                  "S888888888"
 *                                                    "7888888Y
 * ThmclrX - A theme color extractor module for Node.js. "e88j
 *                                                         "Y
 * Copyright (c) 2017 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/thmclrx/blob/master/LICENSE>
 */
#include <byakuren/byakuren.h>
#include <nan.h>
#include "common.h"

namespace __thmclrx__ {

NAN_METHOD(GetByOctree)
{
    if(info.Length() < 1)
    {
        return Nan::ThrowError("Wrong number of arguments.");
    }

    // This function only need one argument
    // -- the RGB object array
    if(!info[0]->IsArray())
    {
        return Nan::ThrowTypeError("The argument should be an array.");
    }

    v8::Local<v8::Array> rgb_array = v8::Local<v8::Array>::Cast(info[0]);
    PicturePixels pixels;
    if(!PicturePixels::CreateFromV8(rgb_array, &pixels))
    {
        return Nan::ThrowTypeError("Failed to recognize pixels.");
    }

    unsigned int max_color = 256;
    if(info.Length() > 1 && info[1]->IsInt32())
    {
        max_color = Nan::To<v8::Int32>(info[1]).ToLocalChecked()->Value();
    }

    bkr_octree_node* root = bkr_build_octree(pixels.colors, pixels.count, max_color);
    bkr_color_stats stats[pixels.count];
    unsigned int color_count = bkr_octree_calculate_color_stats(root, stats);

    bkr_release_octree(root);
    PicturePixels::SafeDestroyInner(&pixels);

    v8::Local<v8::Array> ret = Nan::New<v8::Array>(color_count);
    v8::Local<v8::String> _color_key = Nan::New("color").ToLocalChecked();
    v8::Local<v8::String> _count_key = Nan::New("count").ToLocalChecked();

    v8::Local<v8::Object> obj;
    char exported_color[7];
    for(unsigned int i = 0; i < color_count; i++)
    {
        obj = Nan::New<v8::Object>();
        ((RGB*)&stats[i].color)->ColorString(exported_color);
        obj->Set(_color_key, Nan::New(exported_color).ToLocalChecked());
        obj->Set(_count_key, Nan::New(stats[i].count));

        ret->Set(i, obj);
    }

    info.GetReturnValue().Set(ret);
}

NAN_MODULE_INIT(Init)
{
    bkr_init();

    // Set the function of
    //   "Getting by octree"
    Nan::Set(target, Nan::New("getByOctree").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(GetByOctree)->GetFunction());
}

}

NODE_MODULE(thmclrx, __thmclrx__::Init);
