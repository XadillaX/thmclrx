# ThmclrX

[![thmclrx](http://img.shields.io/npm/v/thmclrx.svg)](https://www.npmjs.org/package/thmclrx)
[![thmclrx](http://img.shields.io/npm/dm/thmclrx.svg)](https://www.npmjs.org/package/thmclrx)

A theme color extractor module for Node.js with ❤ and built on the top of [Byakuren](https://github.com/XadillaX/byakuren).

## Algorithm

- Minimum Differ Algorithm
- Octree Algorithm
- Mixed Algorithm

## Installation

```shell
$ npm install --save thmclrx
```

## API

There only three API in Node.js now.

### octree

`octree` is to get the theme colors in an octree. The colors are unsertain but fidelity.

The function is like below:

```javascript
var thmclrx = require("thmclrx");
thmclrx.octree(file, [maxColors], [callback], [frameNumber]);
```

+ `file`: it could be a local filename, remote url or even an image buffer.
+ `maxColors`: count of max theme colors you want to extract. Defaults to 256;
+ `callback`: it should be like `function(err, colors) {}`. Defaults to an empty function;
+ `frameNumber`: the frame you want to extract. Usually used in `gif`. Defaults to 0.

### minDiff

`minDiff` is to get theme colors in minimum differ algorithm by passing a palette. The result theme colors are certainlly in your palette.

```javascript
var thmclrx = require("thmclrx");
thmclrx.minDiff(file, [palette], [callback], [frameNumber]);
```

+ `file`: it could be a local filename, remote url or even an image buffer.
+ `palette`: palette is an array that in the struct of `[ { r: .., g: .., b: .., }, { r: .., g: .., b: .. } ]`. Default palette refers [here](https://github.com/XadillaX/thmclrx/blob/master/lib/default_palette.js).
+ `callback`: it should be like `function(err, colors) {}`. Defaults to an empty function;
+ `frameNumber`: the frame you want to extract. Usually used in `gif`. Defaults to 0.

### mixed

Using `mixed` get the basic fidelity theme colors and then using `minDiff` to standardize the fidelity theme colors to a certain palette.

```javascript
var thmclrx = require("thmclrx");
thmclrx.mixed(file, [firstStepMaxColors], [palette], [callback], [frameNumber]);
```

+ `file`: same as the two functions above.
+ `firstStepMaxColors`: same as the `maxColors` in `octreeGet`. Defaults to 256.
+ `palette`: same as the `palette` in `mindiffGet`. Same default value.
+ `callback`: same as the two functions above.
+ `frameNumber`: same as the two functions above.

## C++ API

If you want to use C++ API directly, you can refer to this.

### getByOctree

This function is called in `octree` in Node.js API.

```javascript
var thmclrx = require("thmclrx").cpp;
var colors = thmclrx.getByOctree(pixels, [maxColor]);
```

+ `pixels`: this is an array in the struct of `[ { r: .., g: .., b: .., }, { r: .., g: .., b: .. } ]`.
+ `maxColor`: same as the `maxColors` in `octreeGet` of Node.js API. Defaults to 256.
+ `@return`: this function will return the theme colors.

### getByMinDiff

This function is called in `minDiff` in Node.js API.

```javascript
var thmclrx = require("thmclrx").cpp;
var colors = thmclrx.getByMinDiff(pixels, [palette]);
```

+ `pixels`: this may be same as the `pixels` in `octreeGet` of C++ API. Otherwise, it may be the result of `octreeGet` of C++ API.
+ `palette`: same as the `palette` in `mindiffGet` of Node.js API. Same default value.
+ `@return`: this function will return the theme colors.

### getByMixed

This function is called in `mixed` in Node.js API.

```javascript
var thmclrx = require("thmclrx").cpp;
var colors = thmclrx.getByMixed(pixels, maxColors, palette);
```

+ `pixels`: this may be same as the `pixels` in `octreeGet` of C++ API. Otherwise, it may be the result of `octreeGet` of C++ API.
+ `maxColors`: same as the `maxColors` in `octreeGet` of Node.js API.
+ `palette`: same as the `palette` in `mindiffGet` of Node.js API.
+ `@return`: this function will return the theme colors.

## Migrate From 0.x To 1.x

thmclrx 0.x has three APIs which map three APIs in 1.x.

+ octreeGet
+ mindiffGet
+ mixGet

In 1.x, these three APIs still exist but deprecated. They are exactly shallow points of `octree` / `minDiff` and `mixed`.

You may do nothing but we recommend you to rename the functions you called.

But if you're using native APIs (C++ APIs), you should read the [new document](#c-api).

## Contribute

Issues and PRs are welcomed!

「雖然我覺得不怎麼可能有人會關注我」
