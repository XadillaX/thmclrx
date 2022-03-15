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
#include <byakuren/byakuren.h>
#include <nan.h>
#include "common.h"

namespace __thmclrx__ {

using v8::Array;
using v8::Context;
using v8::Local;
using v8::Uint32;

NAN_METHOD(GetByOctree) {
  if (info.Length() < 1) {
    return Nan::ThrowError("Wrong number of arguments.");
  }

  if (!info[0]->IsArray()) {
    return Nan::ThrowTypeError("The argument should be an array.");
  }

  Local<Array> rgb_array = info[0].As<Array>();
  PicturePixels pixels;
  if (!PicturePixels::CreateFromV8(rgb_array, &pixels)) {
    return Nan::ThrowTypeError("Failed to recognize pixels.");
  }

  size_t max_color = 256;
  if (info.Length() > 1 && info[1]->IsUint32()) {
    max_color = Nan::To<Uint32>(info[1]).ToLocalChecked()->Value();
  }

  bkr_octree_node* root =
      bkr_build_octree(pixels.colors, pixels.count, max_color);
  bkr_color_stats* stats = reinterpret_cast<bkr_color_stats*>(
      malloc(sizeof(bkr_color_stats) * max_color));
  size_t color_count = bkr_octree_calculate_color_stats(root, stats);

  bkr_release_octree(root);
  PicturePixels::SafeDestroyInner(&pixels);

  info.GetReturnValue().Set(StatsToV8(stats, color_count));
  free(stats);
}

NAN_METHOD(GetByMinDiff) {
  if (info.Length() < 1) {
    return Nan::ThrowError("Wrong number of arguments.");
  }

  if (!info[0]->IsArray()) {
    return Nan::ThrowTypeError("The argument should be an array.");
  }

  Local<Array> rgb_array = info[0].As<Array>();
  PicturePixels pixels;
  if (!PicturePixels::CreateFromV8(rgb_array, &pixels)) {
    return Nan::ThrowTypeError("Failed to recognize pixels.");
  }

  Palette palette(0, nullptr);
  if (info.Length() != 1) {
    Palette::CreateFromV8(info[1], &palette);
  } else {
    palette = *Palette::GetDefaultPalette();
  }

  bkr_color_stats* stats = reinterpret_cast<bkr_color_stats*>(
      malloc(sizeof(bkr_color_stats) * palette.count));
  bkr_mindiff_parameter param;
  param.gray_offset = 5;
  param.palette = &palette;

  size_t color_count = bkr_mindiff_calculate_color_stats(
      pixels.colors, pixels.count, stats, &param);

  PicturePixels::SafeDestroyInner(&pixels);
  Palette::SafeDestroyInner(&palette);

  info.GetReturnValue().Set(StatsToV8(stats, color_count));
  free(stats);
}

NAN_METHOD(GetByMixed) {
  if (info.Length() < 1) {
    return Nan::ThrowError("Wrong number of arguments.");
  }

  if (!info[0]->IsArray()) {
    return Nan::ThrowTypeError("The argument should be an array.");
  }

  Local<Array> rgb_array = info[0].As<Array>();
  PicturePixels pixels;
  if (!PicturePixels::CreateFromV8(rgb_array, &pixels)) {
    return Nan::ThrowTypeError("Failed to recognize pixels.");
  }

  size_t max_color = 256;
  if (info.Length() > 1 && info[1]->IsUint32()) {
    max_color = Nan::To<Uint32>(info[1]).ToLocalChecked()->Value();
  }

  Palette palette(0, nullptr);
  if (info.Length() >= 3) {
    Palette::CreateFromV8(info[2], &palette);
  } else {
    palette = *Palette::GetDefaultPalette();
  }

  bkr_color_stats* stats = reinterpret_cast<bkr_color_stats*>(
      malloc(sizeof(bkr_color_stats) * palette.count));
  bkr_mindiff_parameter param;
  param.gray_offset = 5;
  param.palette = &palette;

  size_t color_count = bkr_mix_calculate_color_stats(
      pixels.colors, pixels.count, max_color, &param, stats);

  PicturePixels::SafeDestroyInner(&pixels);
  Palette::SafeDestroyInner(&palette);

  info.GetReturnValue().Set(StatsToV8(stats, color_count));
  free(stats);
}

NAN_MODULE_INIT(Init) {
  bkr_init();
  Local<Context> context = target->GetIsolate()->GetCurrentContext();

  // Set the function of
  //   "Getting by octree"
  Nan::Set(target,
           Nan::New("getByOctree").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(GetByOctree)
               ->GetFunction(context)
               .ToLocalChecked());

  // Set the function of
  //   "Getting by mindiff"
  Nan::Set(target,
           Nan::New("getByMinDiff").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(GetByMinDiff)
               ->GetFunction(context)
               .ToLocalChecked());

  // Set the function of
  //   "Getting by mixed"
  Nan::Set(target,
           Nan::New("getByMixed").ToLocalChecked(),
           Nan::New<v8::FunctionTemplate>(GetByMixed)
               ->GetFunction(context)
               .ToLocalChecked());
}

}  // namespace __thmclrx__

NODE_MODULE(thmclrx, __thmclrx__::Init);
