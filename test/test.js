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
'use strict';

const fs = require('fs');
const path = require('path');

const thmclrx = require('../');

function genCallback(id) {
  return (err, result) => {
    if (err) {
      return console.log(err.message);
    }

    let string = '';
    for (let i = 0; i < result.length; i++) {
      string +=
        `<div
           style="width: 50px; height: 21px; float: left; margin-right: 5px;
                  margin-bottom: 5px; background: #${result[i].color};
                  color: #fff; font-size: 12px; text-align: center;
                  padding-top: 9px;"
         >
           ${result[i].count}
         </div>`;
    }

    fs.writeFileSync(`test${id}.html`, string, 'utf8');
    console.log(`done ${id}.`);
  };
}

thmclrx.octreeGet(path.join(__dirname, 'pic.jpg'), 32, genCallback(1));
thmclrx.mindiffGet(path.join(__dirname, '/pic.jpg'), genCallback(2));
thmclrx.mixGet(
  'http://img.hb.aicdn.com/a4bdcf094ff649f7df75ec2bb7c3f3bba5ad53631e9f42-CHoVqn_fw658',
  32,
  genCallback(3));
