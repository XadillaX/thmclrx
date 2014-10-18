/**
 * XadillaX created at 2014-09-15 12:27:23
 *
 * Copyright (c) 2014 Huaban.com, all rights
 * reserved
 */
var sugar = require("sugar");
var thmclrx = require("../");
var fs = require("fs");

thmclrx.octreeGet("pic.jpg", 32, function(err, result) {
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

    thmclrx.cleanPool();
});
