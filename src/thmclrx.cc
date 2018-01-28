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
 * Copyright (c) 2018 XadillaX <i@2333.moe>
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
    bkr_color_stats stats[max_color];
    unsigned int color_count = bkr_octree_calculate_color_stats(root, stats);

    bkr_release_octree(root);
    PicturePixels::SafeDestroyInner(&pixels);

    info.GetReturnValue().Set(StatsToV8(stats, color_count));
}

NAN_METHOD(GetByMinDiff)
{
    if(info.Length() < 1)
    {
        return Nan::ThrowError("Wrong number of arguments.");
    }

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

    Palette palette(0, NULL);
    if(info.Length() != 1)
    {
        Palette::CreateFromV8(info[1], &palette);
    }
    else
    {
        palette = *Palette::GetDefaultPalette();
    }

    bkr_color_stats stats[palette.count];
    bkr_mindiff_parameter param;
    param.gray_offset = 5;
    param.palette = &palette;

    unsigned int color_count = bkr_mindiff_calculate_color_stats(pixels.colors, pixels.count, stats, &param);

    PicturePixels::SafeDestroyInner(&pixels);
    Palette::SafeDestroyInner(&palette);

    info.GetReturnValue().Set(StatsToV8(stats, color_count));
}

NAN_METHOD(GetByMixed)
{
    if(info.Length() < 1)
    {
        return Nan::ThrowError("Wrong number of arguments.");
    }

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

    Palette palette(0, NULL);
    if(info.Length() >= 3)
    {
        Palette::CreateFromV8(info[2], &palette);
    }
    else
    {
        palette = *Palette::GetDefaultPalette();
    }

    bkr_color_stats stats[palette.count];
    bkr_mindiff_parameter param;
    param.gray_offset = 5;
    param.palette = &palette;

    unsigned int color_count = bkr_mix_calculate_color_stats(pixels.colors, pixels.count, max_color, &param, stats);

    PicturePixels::SafeDestroyInner(&pixels);
    Palette::SafeDestroyInner(&palette);

    info.GetReturnValue().Set(StatsToV8(stats, color_count));
}

NAN_MODULE_INIT(Init)
{
    bkr_init();

    // Set the function of
    //   "Getting by octree"
    Nan::Set(target, Nan::New("getByOctree").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(GetByOctree)->GetFunction());

    // Set the function of
    //   "Getting by mindiff"
    Nan::Set(target, Nan::New("getByMinDiff").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(GetByMinDiff)->GetFunction());

    // Set the function of
    //   "Getting by mixed"
    Nan::Set(target, Nan::New("getByMixed").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(GetByMixed)->GetFunction());
}

}

NODE_MODULE(thmclrx, __thmclrx__::Init);
