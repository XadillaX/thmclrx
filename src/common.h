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
#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include <byakuren/byakuren.h>
#include <v8.h>
#include <string>

namespace __thmclrx__ {

typedef struct RGB : bkr_rgb {
  void ColorString(char* str);
} RGB;

typedef struct Palette : bkr_palette_array {
  Palette(size_t count, bkr_rgb* colors);

  // for some functions of V8
  static const Palette* GetDefaultPalette();
  static void CreateFromV8(v8::Local<v8::Value> value, Palette* palette);
  static void SafeDestroyInner(Palette* palette);
} Palette;

typedef struct PicturePixels {
  RGB* colors;
  size_t count;

  // for some functions of V8
  static bool CreateFromV8(v8::Local<v8::Value> value, PicturePixels* pixels);
  static void SafeDestroyInner(PicturePixels* pixels);
} PicturePixels;

v8::Local<v8::Array> StatsToV8(bkr_color_stats stats[], size_t count);

}  // namespace __thmclrx__

#endif  // SRC_COMMON_H_
