/**
 * XadillaX created at 2014-09-15 12:27:23
 *
 * Copyright (c) 2014 Huaban.com, all rights
 * reserved
 */
var sugar = require("sugar");
var thmclrx = require("../");
var fs = require("fs");

thmclrx.octreeGet("pic.jpg", 8, function(err, result) {
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

thmclrx.mindiffGet("pic.jpg", function(err, result) {
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
