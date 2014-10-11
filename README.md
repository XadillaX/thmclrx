# ThmclrX

A theme color extractor module for Node.js.

## Algorithm

+ Minimum Differ Algorithm
+ Octree Algorithm

## Installation

```sh
$ npm install thmclrx
```

## API

There only three API in node.js now.

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

`mindiffGet` is to get theme colors in minimum differ algorithm by passing a palette. The result theme colors are certainlly in your palette.

```javascript
var thmclrx = require("thmclrx");
thmclrx.mindiffGet(file, [palette], [callback], [frameNumber]);
```

+ `file`: it could be a local filename, remote url or even an image buffer.
+ `palette`: palette is an array that in the struct of `[ { r: .., g: .., b: .., }, { r: .., g: .., b: .. } ]`. Default palette refers [here](https://github.com/XadillaX/thmclrx/blob/master/lib/defaultPalette.js).
+ `callback`: it should be like `function(err, colors) {}`. Defaults to an empty function;
+ `frameNumber`: the frame you want to extract. Usually used in `gif`. Defaults to 0.

### mixGet

Using `octreeGet` get the basic fidelity theme colors and then using `mindiffGet` to standardize the fidelity theme colors to a certain palette.

```javascript
var thmclrx = require("thmclrx");
thmclrx.mixGet(file, [firstStepMaxColors], [palette], [callback], [frameNumber]);
```

+ `file`: same as the two functions above.
+ `firstStepMaxColors`: same as the `maxColors` in `octreeGet`. Defaults to 256.
+ `palette`: same as the `palette` in `mindiffGet`. Same default value.
+ `callback`: same as the two functions above.
+ `frameNumber`: same as the two functions above.

### cleanPool

Clean memory pool in `thmclrx` C++ program.

```javascript
var thmclrx = require("thmclrx");
thmclrx.cleanPool();
```

> Normally, you do not need to call it.

## C++ API

If you want to use C++ API directly, you can refer to this.

### octreeGet

This function is called in `octreeGet` in node.js API.

```javascript
var thmclrx = require("thmclrx/build/Release/thmclrx.node");
var colors = thmclrx.octreeGet(pixels, [maxColor]);
```

+ `pixels`: this is an array in the struct of `[ { r: .., g: .., b: .., }, { r: .., g: .., b: .. } ]`.
+ `maxColor`: same as the `maxColors` in `octreeGet` of node.js API. Defaults to 256.
+ `@return`: this function will return the theme colors.

### mindifferGet

This function is called in `mindiffGet` in node.js API.

```javascript
var thmclrx = require("thmclrx/build/Release/thmclrx.node");
var colors = thmclrx.mindifferGet(pixels, [palette]);
```

+ `pixels`: this may be same as the `pixels` in `octreeGet` of C++ API. Otherwise, it may be the result of `octreeGet` of C++ API.
+ `palette`: same as the `palette` in `mindiffGet` of node.js API. Same default value.
+ `@return`: this function will return the theme colors.

### cleanPool

Clean memory pool.

```javascript
var thmclrx = require("thmclrx/build/Release/thmclrx.node");
thmclrx.clearPool();
```

> Normally, you do not need to call it.

## Contribute

You're welcome to fork and pull requests!

