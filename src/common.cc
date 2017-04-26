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
#include <nan.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

namespace __thmclrx__ {

const Palette default_palette(256, _default_palette.colors);

void RGB::ColorString(char* str)
{
    sprintf(str, "%.2X%.2X%.2X", this->red, this->green, this->blue);
}

Palette::Palette(int count, bkr_rgb* colors)
{
    this->count = count;
    this->colors = colors;
}

const Palette* Palette::GetDefaultPalette()
{
    return &default_palette;
}

void Palette::CreateFromV8(v8::Local<v8::Value> value, Palette* palette)
{
    Nan::HandleScope scope;

    if(!value->IsArray())
    {
        memcpy(palette, &default_palette, sizeof(Palette));
        return;
    }

    v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(value);
    RGB* rgb_array = new RGB[array->Length()];

    for(unsigned int i = 0; i < array->Length(); i++)
    {
        if(!array->Get(i)->IsObject())
        {
            delete []rgb_array;
            memcpy(palette, &default_palette, sizeof(Palette));
            return;
        }

        v8::Local<v8::Object> object = array->Get(i)->ToObject();
        v8::Local<v8::String> _r_key = Nan::New("r").ToLocalChecked();
        v8::Local<v8::Value> r = object->Get(_r_key);
        v8::Local<v8::String> _g_key = Nan::New("g").ToLocalChecked();
        v8::Local<v8::Value> g = object->Get(_g_key);
        v8::Local<v8::String> _b_key = Nan::New("b").ToLocalChecked();
        v8::Local<v8::Value> b = object->Get(_b_key);

        if(!r->IsInt32() || !g->IsInt32() || !b->IsInt32())
        {
            delete []rgb_array;
            memcpy(palette, &default_palette, sizeof(Palette));
            return;
        }

        rgb_array[i].red = (unsigned char)Nan::To<v8::Int32>(r).ToLocalChecked()->Value();
        rgb_array[i].green = (unsigned char)Nan::To<v8::Int32>(g).ToLocalChecked()->Value();
        rgb_array[i].blue = (unsigned char)Nan::To<v8::Int32>(b).ToLocalChecked()->Value();
    }

    palette->count = array->Length();
    palette->colors = rgb_array;
}

void Palette::SafeDestroyInner(Palette* palette)
{
    if(palette->colors != NULL && palette->colors != default_palette.colors)
    {
        delete []palette->colors;
    }

    palette->colors = NULL;
    palette->count = 0;
}

bool PicturePixels::CreateFromV8(v8::Local<v8::Value> value, PicturePixels* pixels)
{
    Nan::HandleScope scope;

    if(!value->IsArray())
    {
        return false;
    }

    v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(value);
    RGB* rgb_array = new RGB[array->Length()];

    for(unsigned int i = 0; i < array->Length(); i++)
    {
        if(!array->Get(i)->IsObject())
        {
            delete []rgb_array;
            return false;
        }

        v8::Local<v8::Object> object = array->Get(i)->ToObject();
        v8::Local<v8::String> _r_key = Nan::New("r").ToLocalChecked();
        v8::Local<v8::Value> r = object->Get(_r_key);
        v8::Local<v8::String> _g_key = Nan::New("g").ToLocalChecked();
        v8::Local<v8::Value> g = object->Get(_g_key);
        v8::Local<v8::String> _b_key = Nan::New("b").ToLocalChecked();
        v8::Local<v8::Value> b = object->Get(_b_key);

        if(!r->IsInt32() || !g->IsInt32() || !b->IsInt32())
        {
            delete []rgb_array;
            return false;
        }

        rgb_array[i].red = (unsigned char)Nan::To<v8::Int32>(r).ToLocalChecked()->Value();
        rgb_array[i].green = (unsigned char)Nan::To<v8::Int32>(g).ToLocalChecked()->Value();
        rgb_array[i].blue = (unsigned char)Nan::To<v8::Int32>(b).ToLocalChecked()->Value();
    }

    pixels->count = array->Length();
    pixels->colors = rgb_array;

    return true;
}

void PicturePixels::SafeDestroyInner(PicturePixels* pixels)
{
    if(pixels->colors != NULL)
    {
        delete []pixels->colors;
    }

    pixels->colors = NULL;
    pixels->count = 0;
}

v8::Local<v8::Array> StatsToV8(bkr_color_stats stats[], unsigned int count)
{
    Nan::HandleScope scope;

    v8::Local<v8::Array> ret = Nan::New<v8::Array>(count);
    v8::Local<v8::String> _color_key = Nan::New("color").ToLocalChecked();
    v8::Local<v8::String> _count_key = Nan::New("count").ToLocalChecked();

    v8::Local<v8::Object> obj;
    char exported_color[7];
    for(unsigned int i = 0; i < count; i++)
    {
        obj = Nan::New<v8::Object>();
        ((RGB*)&stats[i].color)->ColorString(exported_color);
        obj->Set(_color_key, Nan::New(exported_color).ToLocalChecked());
        obj->Set(_count_key, Nan::New(stats[i].count));

        ret->Set(i, obj);
    }

    return ret;
}

}
