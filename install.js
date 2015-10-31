#!/usr/bin/env node

var os = require('os');
var path = require('path');
var fs = require('fs');
var nugget = require('nugget');
var extract = require('extract-zip');

var platform = os.platform();
var arch = os.arch();
if (platform === 'win32') {
  // 64-bit is not available under windows.
  arch = 'ia32';
}

var project = 'https://github.com/luuvish/node-ffmpeg';
var url = project + '/releases/download/ffmpeg-2.6/';
var version = '2.6';
var filename = 'ffmpeg-' + version + '-' + platform + '-' + arch + '.zip';

function onerror (err) {
  throw err;
}

var config = {
  target: filename,
  dir: path.join(__dirname, 'dist'),
  resume: true,
  verbose: true
};

if (!fs.existsSync(config.dir)) {
  fs.mkdirSync(config.dir);
}

nugget(url + filename, config, function (err) {
  if (err) return onerror(err);
  var source = path.join(__dirname, 'dist', filename);
  var target = path.join(__dirname, 'dist', 'ffmpeg-2.6');
  extract(source, {dir: target}, function (err) {
    if (err) return onerror(err);
  });
});
