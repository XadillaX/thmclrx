/**
 *
 *    ▄▄▄▄▀ ▄  █ █▀▄▀█ ▄█▄    █    █▄▄▄▄     ▄            ,ae,
 * ▀▀▀ █   █   █ █ █ █ █▀ ▀▄  █    █  ▄▀ ▀▄   █          ,88888e
 *     █   ██▀▀█ █ ▄ █ █   ▀  █    █▀▀▌    █ ▀    ,a888b.9888888i
 *    █    █   █ █   █ █▄  ▄▀ ███▄ █  █   ▄ █     888888888888888
 *   ▀        █     █  ▀███▀      ▀  █   █   ▀▄   88888888888888Y
 *           ▀     ▀                ▀     ▀       '8888888888888'
 *                                                  'S888888888'
 *                                                    '7888888Y
 * ThmclrX - A theme color extractor module for Node.js. 'e88j
 *                                                         'Y
 * Copyright (c) 2022 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/thmclrx/blob/master/LICENSE>
 */
'use strict';

const pixel = require('pixel-getter');

const _defaultPalette = require('./default_palette');
const cpp = require('../build/Release/thmclrx.node');

const emptyCallback = () => {};
const defaultPalette = _defaultPalette.map(palette => ({
  r: palette[0],
  g: palette[1],
  b: palette[2],
}));

/**
 * Get theme color by octree algorithm.
 * @param {string} file The uri of a file.
 * @param {number} [maxColors] The max color count of the result should return.
 * @param {(err: Error | undefined, colors?: { r: number, g: number, b: number }[]) => void} [callback] The callback function
 * @param {number} [frameNumber] The frame number if it's a GIF.
 */
exports.octree = function(file, maxColors, callback, frameNumber) {
  if (typeof callback === 'number') {
    frameNumber = callback;
    callback = undefined;
  }

  if (typeof maxColors === 'function') {
    callback = maxColors;
    maxColors = undefined;
  }

  if (undefined === frameNumber) frameNumber = 0;
  if (undefined === callback) callback = emptyCallback;
  if (undefined === maxColors) maxColors = 256;

  pixel.get(file, (err, pixels) => {
    if (err) {
      return callback(err);
    }

    let result;
    try {
      result = cpp.getByOctree(pixels[frameNumber], maxColors);
      pixels = undefined;
    } catch (e) {
      return callback(e);
    }

    callback(undefined, result);
  });
};

/**
 * Get theme color by mindiff algorithm.
 * @param {string} file The uri of a file.
 * @param {{ r: number, g: number, b: number }[]} [palette] The palette array.
 * @param {(err: Error | undefined, colors?: { r: number, g: number, b: number }[]) => void} [callback] The callback function.
 * @param {number} [frameNumber] The frame number if it's a GIF.
 */
exports.minDiff = function(file, palette, callback, frameNumber) {
  if (typeof callback === 'number') {
    frameNumber = callback;
    callback = undefined;
  }

  if (typeof palette === 'function') {
    callback = palette;
    palette = undefined;
  }

  if (undefined === frameNumber) frameNumber = 0;
  if (undefined === callback) callback = emptyCallback;
  if (undefined === palette) palette = defaultPalette;

  pixel.get(file, (err, pixels) => {
    if (err) {
      return callback(err);
    }

    let result;
    try {
      result = cpp.getByMinDiff(pixels[frameNumber], palette);
      pixels = undefined;
    } catch (e) {
      return callback(e);
    }

    callback(undefined, result);
  });
};

/**
 * Get theme color by mixed algorithm.
 * @param {string} file The uri of a file.
 * @param {number} [firstStepMaxColors] The max colors should return.
 * @param {{ r: number, g: number, b: number }[]} [palette] The palette array.
 * @param {(err: Error | undefined, colors?: { r: number, g: number, b: number }[]) => void} [callback] The callback function.
 * @param {number} [frameNumber] The frame number if it's a GIF.
 */
exports.mixed = function(file, firstStepMaxColors, palette, callback, frameNumber) {
  if (typeof callback === 'number') {
    frameNumber = callback;
    callback = undefined;
  }

  if (typeof palette === 'number') {
    frameNumber = palette;
    palette = undefined;
  }

  if (typeof palette === 'function') {
    callback = palette;
    palette = undefined;
  }

  if (typeof firstStepMaxColors === 'function') {
    callback = firstStepMaxColors;
    firstStepMaxColors = undefined;
  }

  if (typeof firstStepMaxColors === 'object') {
    palette = firstStepMaxColors;
    firstStepMaxColors = undefined;
  }

  if (undefined === firstStepMaxColors) firstStepMaxColors = 256;
  if (undefined === palette) palette = defaultPalette;
  if (undefined === callback) callback = emptyCallback;
  if (undefined === frameNumber) frameNumber = 0;

  pixel.get(file, (err, pixels) => {
    if (err) {
      return callback(err);
    }

    let result;
    try {
      result = cpp.getByMixed(pixels[frameNumber], firstStepMaxColors, palette);
    } catch (e) {
      return callback(e);
    }

    callback(undefined, result);
  });
};

// adapt for 0.*
exports.octreeGet = exports.octree;
exports.mindiffGet = exports.minDiff;
exports.mixGet = exports.mixed;
