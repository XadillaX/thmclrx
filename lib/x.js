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
"use strict";

var thmclrxCpp = require("../build/Release/thmclrx.node");
var pixelGetter = require("pixel-getter");
var defaultPalette = require("./default_palette");
defaultPalette = defaultPalette.map(function(palette) {
    return {
        r   : palette[0],
        g   : palette[1],
        b   : palette[2]
    };
});

/**
 * octree get
 * @param file
 * @param [maxColors]
 * @param [callback]
 * @param [frameNumber]
 */
exports.octreeGet = function(file, maxColors, callback, frameNumber) {
    if(typeof callback === "number") {
        frameNumber = callback;
        callback = undefined;
    }

    if(typeof maxColors === "function") {
        callback = maxColors;
        maxColors = undefined;
    }

    if(undefined === frameNumber) frameNumber = 0;
    if(undefined === callback) callback = function(){};
    if(undefined === maxColors) maxColors = 256;

    pixelGetter.get(file, function(err, pixels) {
        if(err) {
            return callback(err);
        }

        try {
            var result = thmclrxCpp.getByOctree(pixels[frameNumber], maxColors);
            pixels = undefined;
        } catch(e) {
            return callback(e);
        }

        callback(undefined, result);
    });
};

/**
 * min diff get
 * @param file
 * @param [palette]
 * @param [callback]
 * @param [frameNumber]
 */
exports.mindiffGet = function(file, palette, callback, frameNumber) {
    if(typeof callback === "number") {
        frameNumber = callback;
        callback = undefined;
    }

    if(typeof palette === "function") {
        callback = palette;
        palette = undefined;
    }

    if(undefined === frameNumber) frameNumber = 0;
    if(undefined === callback) callback = function(){};
    if(undefined === palette) palette = defaultPalette;

    pixelGetter.get(file, function(err, pixels) {
        if(err) {
            return callback(err);
        }

        try {
            var result = thmclrxCpp.getByMinDiff(pixels[frameNumber], palette);
            pixels = undefined;
        } catch(e) {
            return callback(e);
        }

        callback(undefined, result);
    });
};

/**
 * mix get
 * @param file
 * @param [firstStepMaxColors]
 * @param [palette]
 * @param [callback]
 * @param [frameNumber]
 */
exports.mixGet = function(file, firstStepMaxColors, palette, callback, frameNumber) {
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
    if(undefined === callback) callback = function(){};
    if(undefined === frameNumber) frameNumber = 0;

    pixelGetter.get(file, function(err, pixels) {
        if(err) {
            return callback(err);
        }

        try {
            var result = thmclrxCpp.getByMixed(pixels[frameNumber], firstStepMaxColors, palette);
        } catch(e) {
            return callback(e);
        }

        callback(undefined, result);
    });
};
