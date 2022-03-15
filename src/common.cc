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
 * Copyright (c) 2022 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/thmclrx/blob/master/LICENSE>
 */
#include "common.h"
#include <nan.h>
#include <stdlib.h>
#include <string.h>

namespace __thmclrx__ {

using Nan::HandleScope;
using v8::Array;
using v8::Persistent;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Uint32;
using v8::Value;

const size_t kColorStringLength = 6;
const Palette kDefaultPalette(256, _default_palette.colors);

Persistent<String> kRedKey;
Persistent<String> kGreenKey;
Persistent<String> kBlueKey;

#define COLOR_KEY(key, camel_key, value)                                       \
  inline Local<String> key##_key() {                                           \
    if (k##camel_key##Key.IsEmpty()) {                                         \
      Local<String> value = Nan::New<String>(#value).ToLocalChecked();         \
      k##camel_key##Key.Reset(Nan::GetCurrentContext()->GetIsolate(), value);  \
      return value;                                                            \
    }                                                                          \
                                                                               \
    return Local<String>::New(Nan::GetCurrentContext()->GetIsolate(),          \
                              k##camel_key##Key);                              \
  }

COLOR_KEY(red, Red, r);
COLOR_KEY(green, Green, g);
COLOR_KEY(blue, Blue, b);

void RGB::ColorString(char* str) {
  snprintf(str,
           sizeof(char) * (kColorStringLength + 1),
           "%.2X%.2X%.2X",
           this->red,
           this->green,
           this->blue);
}

Palette::Palette(size_t count, bkr_rgb* colors) {
  this->count = count;
  this->colors = colors;
}

const Palette* Palette::GetDefaultPalette() {
  return &kDefaultPalette;
}

void Palette::CreateFromV8(Local<Value> value, Palette* palette) {
  HandleScope scope;

  if (!value->IsArray()) {
    memcpy(palette, &kDefaultPalette, sizeof(Palette));
    return;
  }

  Local<Array> array = value.As<Array>();
  RGB* rgb_array = new RGB[array->Length()];

  size_t array_length = array->Length();
  for (size_t i = 0; i < array_length; i++) {
    Local<Value> value = Nan::Get(array, i).ToLocalChecked();
    if (!value->IsObject()) {
      delete[] rgb_array;
      memcpy(palette, &kDefaultPalette, sizeof(Palette));
      return;
    }

    Local<Object> object = Nan::To<Object>(value).ToLocalChecked();
    Local<Value> r = Nan::Get(object, red_key()).ToLocalChecked();
    Local<Value> g = Nan::Get(object, green_key()).ToLocalChecked();
    Local<Value> b = Nan::Get(object, blue_key()).ToLocalChecked();

    if (!r->IsUint32() || !g->IsUint32() || !b->IsUint32()) {
      delete[] rgb_array;
      memcpy(palette, &kDefaultPalette, sizeof(Palette));
      return;
    }

    rgb_array[i].red = static_cast<unsigned char>(
        Nan::To<Uint32>(r).ToLocalChecked()->Value());
    rgb_array[i].green = static_cast<unsigned char>(
        Nan::To<Uint32>(g).ToLocalChecked()->Value());
    rgb_array[i].blue = static_cast<unsigned char>(
        Nan::To<Uint32>(b).ToLocalChecked()->Value());
  }

  palette->count = array->Length();
  palette->colors = rgb_array;
}

void Palette::SafeDestroyInner(Palette* palette) {
  if (palette->colors != nullptr && palette->colors != kDefaultPalette.colors) {
    delete[] palette->colors;
  }

  palette->colors = nullptr;
  palette->count = 0;
}

bool PicturePixels::CreateFromV8(Local<v8::Value> value,
                                 PicturePixels* pixels) {
  HandleScope scope;

  if (!value->IsArray()) {
    return false;
  }

  Local<Array> array = value.As<Array>();
  RGB* rgb_array = new RGB[array->Length()];

  size_t array_length = array->Length();
  for (size_t i = 0; i < array_length; i++) {
    Local<Value> value = Nan::Get(array, i).ToLocalChecked();
    if (!value->IsObject()) {
      delete[] rgb_array;
      return false;
    }

    v8::Local<v8::Object> object = Nan::To<Object>(value).ToLocalChecked();
    v8::Local<v8::Value> r = Nan::Get(object, red_key()).ToLocalChecked();
    v8::Local<v8::Value> g = Nan::Get(object, green_key()).ToLocalChecked();
    v8::Local<v8::Value> b = Nan::Get(object, blue_key()).ToLocalChecked();

    if (!r->IsUint32() || !g->IsUint32() || !b->IsUint32()) {
      delete[] rgb_array;
      return false;
    }

    rgb_array[i].red = static_cast<unsigned char>(
        Nan::To<Uint32>(r).ToLocalChecked()->Value());
    rgb_array[i].green = static_cast<unsigned char>(
        Nan::To<Uint32>(g).ToLocalChecked()->Value());
    rgb_array[i].blue = static_cast<unsigned char>(
        Nan::To<Uint32>(b).ToLocalChecked()->Value());
  }

  pixels->count = array->Length();
  pixels->colors = rgb_array;

  return true;
}

void PicturePixels::SafeDestroyInner(PicturePixels* pixels) {
  if (pixels->colors != nullptr) {
    delete[] pixels->colors;
  }

  pixels->colors = nullptr;
  pixels->count = 0;
}

Local<Array> StatsToV8(bkr_color_stats stats[], size_t count) {
  HandleScope scope;

  v8::Local<v8::Array> ret = Nan::New<Array>(count);
  v8::Local<v8::String> color_key = Nan::New("color").ToLocalChecked();
  v8::Local<v8::String> count_key = Nan::New("count").ToLocalChecked();

  v8::Local<v8::Object> obj;
  char exported_color[7];
  for (size_t i = 0; i < count; i++) {
    obj = Nan::New<v8::Object>();
    (reinterpret_cast<RGB*>(&stats[i].color))->ColorString(exported_color);
    Nan::Set(obj, color_key, Nan::New(exported_color).ToLocalChecked());
    Nan::Set(obj, count_key, Nan::New(stats[i].count));
    Nan::Set(ret, i, obj);
  }

  return ret;
}

}  // namespace __thmclrx__
