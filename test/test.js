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

var thmclrx = require("../");
var fs = require("fs");

thmclrx.octreeGet(__dirname + "/pic.jpg", 32, function(err, result) {
    if(err) {
        return console.log(err.message);
    }

    var string = "";
    for(var i = 0; i < result.length; i++) {
        string += "<div style=\"width: 50px; height: 21px; float: left; margin-right: 5px; margin-bottom: 5px; background: #"
            + result[i].color + "; color: #fff; font-size: 12px; text-align: center; padding-top: 9px;\">" + result[i].count + "</div>";
    }

    fs.writeFileSync("test1.html", string, "utf8");
    console.log("done.");
});

thmclrx.mindiffGet(__dirname + "/pic.jpg", function(err, result) {
     if(err) {
        return console.log(err.message);
    }

    var string = "";
    for(var i = 0; i < result.length; i++) {
        string += "<div style=\"width: 50px; height: 21px; float: left; margin-right: 5px; margin-bottom: 5px; background: #"
            + result[i].color + "; color: #fff; font-size: 12px; text-align: center; padding-top: 9px;\">" + result[i].count + "</div>";
    }

    fs.writeFileSync("test2.html", string, "utf8");
    console.log("done 2.");
});

thmclrx.mixGet("http://img.hb.aicdn.com/a4bdcf094ff649f7df75ec2bb7c3f3bba5ad53631e9f42-CHoVqn_fw658", 32, function(err, result) {
    if(err) {
        return console.log(err.message);
    }

    var string = "";
    for(var i = 0; i < result.length; i++) {
        string += "<div style=\"width: 50px; height: 21px; float: left; margin-right: 5px; margin-bottom: 5px; background: #"
            + result[i].color + "; color: #fff; font-size: 12px; text-align: center; padding-top: 9px;\">" + result[i].count + "</div>";
    }

    fs.writeFileSync("test3.html", string, "utf8");
    console.log("done 3.");

    // thmclrx.cleanPool();
});
