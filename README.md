node-ffmpeg
===========

FFmpeg node modules


## install (Mac OS X)

`git clone https://github.com/luuvish/node-ffmpeg`
`npm install`


## usage ffmpeg module

var ffmpeg = require('ffmpeg');
var fc = ffmpeg.AVFormatContext();

fc.openInput('./test/test.mp4');
