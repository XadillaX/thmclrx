/**
 * XadillaX created at 2014-09-16 13:09
 *
 * Copyright (c) 2014 Huaban.com, all rights
 * reserved.
 */
require("sugar");
var thmclrxCpp = require("../build/Release/thmclrx.node");
var pixelGetter = require("pixel-getter");
var defaultPalette = require("./defaultPalette");
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
            var result = thmclrxCpp.octreeGet(pixels[frameNumber], maxColors);
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
            var result = thmclrxCpp.mindifferGet(pixels[frameNumber], palette);
        } catch(e) {
            return callback(e);
        }

        callback(undefined, result);
    });
};
