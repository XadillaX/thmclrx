#!/usr/bin/env node
'use strict';

const cp = require('child_process');

const bin = require.resolve('clang-format/index');
const args = process.argv.slice(2);

cp.execFileSync(bin, args);
