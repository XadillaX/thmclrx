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
"use strict";

var cpp = require("../build/Release/thmclrx.node");
var pixel = require("pixel-getter");
var defaultPalette = require("./default_palette");

function _emptyCallback() {}

defaultPalette = defaultPalette.map(function(palette) {
    return {
        r: palette[0],
        g: palette[1],
        b: palette[2]
    };
});

/**
 * Get theme color by octree
 * @param {String} file the uri of a file
 * @param {Number} [maxColors] the max color count of the result should return
 * @param {Function} [callback] the callback function
 * @param {Number} [frameNumber] the frame number if it's a GIF
 */
exports.octree = function(file, maxColors, callback, frameNumber) {
    if(typeof callback === "number") {
        frameNumber = callback;
        callback = undefined;
    }

    if(typeof maxColors === "function") {
        callback = maxColors;
        maxColors = undefined;
    }

    if(undefined === frameNumber) frameNumber = 0;
    if(undefined === callback) callback = _emptyCallback;
    if(undefined === maxColors) maxColors = 256;

    pixel.get(file, function(err, pixels) {
        if(err) {
            return callback(err);
        }

        try {
            var result = cpp.getByOctree(pixels[frameNumber], maxColors);
            pixels = undefined;
        } catch(e) {
            return callback(e);
        }

        callback(undefined, result);
    });
};

/**
 * Get theme color by mindiff
 * @param {String} file the uri of a file
 * @param {Array} [palette] the palette array
 * @param {Function} [callback] the callback function
 * @param {Number} [frameNumber] the frame number if it's a GIF
 */
exports.minDiff = function(file, palette, callback, frameNumber) {
    if(typeof callback === "number") {
        frameNumber = callback;
        callback = undefined;
    }

    if(typeof palette === "function") {
        callback = palette;
        palette = undefined;
    }

    if(undefined === frameNumber) frameNumber = 0;
    if(undefined === callback) callback = _emptyCallback;
    if(undefined === palette) palette = defaultPalette;

    pixel.get(file, function(err, pixels) {
        if(err) {
            return callback(err);
        }

        try {
            var result = cpp.getByMinDiff(pixels[frameNumber], palette);
            pixels = undefined;
        } catch(e) {
            return callback(e);
        }

        callback(undefined, result);
    });
};

/**
 * Get theme color by mixed
 * @param {String} file the uri of a file
 * @param {Number} [firstStepMaxColors] the max colors should return
 * @param {Array} [palette] the palette array
 * @param {Function} [callback] the callback function
 * @param {Number} [frameNumber] the frame number if it's a GIF
 */
exports.mixed = function(file, firstStepMaxColors, palette, callback, frameNumber) {
    if(typeof callback === "number") {
        frameNumber = callback;
        callback = undefined;
    }

    if(typeof palette === "number") {
        frameNumber = palette;
        palette = undefined;
    }

    if(typeof palette === "function") {
        callback = palette;
        palette = undefined;
    }

    if(typeof firstStepMaxColors === "function") {
        callback = firstStepMaxColors;
        firstStepMaxColors = undefined;
    }

    if(typeof firstStepMaxColors === "object") {
        palette = firstStepMaxColors;
        firstStepMaxColors = undefined;
    }

    if(undefined === firstStepMaxColors) firstStepMaxColors = 256;
    if(undefined === palette) palette = defaultPalette;
    if(undefined === callback) callback = _emptyCallback;
    if(undefined === frameNumber) frameNumber = 0;

    pixel.get(file, function(err, pixels) {
        if(err) {
            return callback(err);
        }

        try {
            var result = cpp.getByMixed(pixels[frameNumber], firstStepMaxColors, palette);
        } catch(e) {
            return callback(e);
        }

        callback(undefined, result);
    });
};

// adapt for 0.*
exports.octreeGet = exports.octree;
exports.mindiffGet = exports.minDiff;
exports.mixGet = exports.mixed;
