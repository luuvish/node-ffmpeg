(function() {
  var ffmpeg = require('bindings')('ffmpeg.node');
  module.exports = ffmpeg;
}).call(this);
