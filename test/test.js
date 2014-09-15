/**
 * XadillaX created at 2014-09-15 12:27:23
 *
 * Copyright (c) 2014 Huaban.com, all rights
 * reserved
 */
var thmclrx = require("../build/Release/thmclrx.node");
var getPixel = require("get-pixels");
var fs = require("fs");

getPixel("pic.jpg", function(err, pixels) {
    var data = pixels.data;
    var array = [];
    for(var i = 0; i < data.length; i += 4) {
        var r = data.readUInt8(i);
        var g = data.readUInt8(i + 1);
        var b = data.readUInt8(i + 2);
        array.push({ r: r, g: g, b: b });
    }

    result = thmclrx.octreeGet(array, 8);
    console.log("done");

    var string = "";
    for(var i = 0; i < result.length; i++) {
        string += "<div style=\"width: 50px; height: 21px; float: left; margin-right: 5px; margin-bottom: 5px; background: #" + result[i].color + "; color: #fff; font-size: 12px; text-align: center; padding-top: 9px;\">" + result[i].count + "</div>";
    }

    fs.writeFileSync("test.html", string, "utf8");

    result = thmclrx.mindifferGet(result);
    string = "";
    for(var i = 0; i < result.length; i++) {
        string += "<div style=\"width: 50px; height: 21px; float: left; margin-right: 5px; margin-bottom: 5px; background: #" + result[i].color + "; color: #fff; font-size: 12px; text-align: center; padding-top: 9px;\">" + result[i].count + "</div>";
    }

    fs.writeFileSync("test2.html", string, "utf8");
    console.log("done");

    process.exit(0);
});

