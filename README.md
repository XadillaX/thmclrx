# ThmclrX

[![thmclrx](http://img.shields.io/npm/v/thmclrx.svg)](https://www.npmjs.org/package/thmclrx)
[![thmclrx](http://img.shields.io/npm/dm/thmclrx.svg)](https://www.npmjs.org/package/thmclrx)

A theme color extractor module for Node.js with ❤ and built on the top of [Byakuren](https://github.com/XadillaX/byakuren).

## Algorithm

+ [x] Minimum Differ Algorithm
+ [x] Octree Algorithm
+ [x] Mixed Algorithm

## Installation

```sh
$ npm install thmclrx --save
```

## API

There only three API in Node.js now.

### octreeGet

`octreeGet` is to get the theme colors in an octree. The colors are unsertain but fidelity.

The function is like below:

```javascript
var thmclrx = require("thmclrx");
thmclrx.octreeGet(file, [maxColors], [callback], [frameNumber]);
```

+ `file`: it could be a local filename, remote url or even an image buffer.
+ `maxColors`: count of max theme colors you want to extract. Defaults to 256;
+ `callback`: it should be like `function(err, colors) {}`. Defaults to an empty function;
+ `frameNumber`: the frame you want to extract. Usually used in `gif`. Defaults to 0.

### mindiffGet

`mindiffGet` is to get theme colors in minimum differ algorithm by passing a palette.
The result theme colors are certainlly in your palette.

```javascript
var thmclrx = require("thmclrx");
thmclrx.mindiffGet(file, [palette], [callback], [frameNumber]);
```

+ `file`: it could be a local filename, remote url or even an image buffer.
+ `palette`: palette is an array that in the struct of `[ { r: .., g: .., b: .., },
  { r: .., g: .., b: .. } ]`. Default palette refers
  [here](https://github.com/XadillaX/thmclrx/blob/master/lib/defaultPalette.js).
+ `callback`: it should be like `function(err, colors) {}`. Defaults to an empty function;
+ `frameNumber`: the frame you want to extract. Usually used in `gif`. Defaults to 0.

### mixGet

Using `octreeGet` get the basic fidelity theme colors and then using `mindiffGet`
to standardize the fidelity theme colors to a certain palette.

```javascript
var thmclrx = require("thmclrx");
thmclrx.mixGet(file, [firstStepMaxColors], [palette], [callback], [frameNumber]);
```

+ `file`: same as the two functions above.
+ `firstStepMaxColors`: same as the `maxColors` in `octreeGet`. Defaults to 256.
+ `palette`: same as the `palette` in `mindiffGet`. Same default value.
+ `callback`: same as the two functions above.
+ `frameNumber`: same as the two functions above.

## C++ API

If you want to use C++ API directly, you can refer to this.

### getByOctree

This function is called in `octreeGet` in Node.js API.

```javascript
var thmclrx = require("thmclrx").cpp;
var colors = thmclrx.getByOctree(pixels, [maxColor]);
```

+ `pixels`: this is an array in the struct of `[ { r: .., g: .., b: .., },
  { r: .., g: .., b: .. } ]`.
+ `maxColor`: same as the `maxColors` in `octreeGet` of Node.js API. Defaults to 256.
+ `@return`: this function will return the theme colors.

### getByMinDiff

This function is called in `mindiffGet` in Node.js API.

```javascript
var thmclrx = require("thmclrx").cpp;
var colors = thmclrx.getByMinDiff(pixels, [palette]);
```

+ `pixels`: this may be same as the `pixels` in `octreeGet` of C++ API. Otherwise,
  it may be the result of `octreeGet` of C++ API.
+ `palette`: same as the `palette` in `mindiffGet` of Node.js API. Same default value.
+ `@return`: this function will return the theme colors.

### getByMixed

This function is called in `mixGet` in Node.js API.

```javascript
var thmclrx = require("thmclrx").cpp;
var colors = thmclrx.getByMixed(pixels, maxColors, palette);
```

+ `pixels`: this may be same as the `pixels` in `octreeGet` of C++ API. Otherwise,
  it may be the result of `octreeGet` of C++ API.
+ `maxColors`: same as the `maxColors` in `octreeGet` of Node.js API.
+ `palette`: same as the `palette` in `mindiffGet` of Node.js API.
+ `@return`: this function will return the theme colors.

## Contribute

You're welcome to fork and make pull requests!

