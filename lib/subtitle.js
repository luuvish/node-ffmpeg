
var FFmpeg = require('./ffmpeg');

var Subtitle = function (format) {
  this.format = format;
  this.options = format.options;

  this.index = -1;
  this.stream = null;
  this.context = null;
};

Subtitle.prototype.open = function (stream) {
  var context = stream.codec;
  var codec = FFmpeg.AVCodec.findDecoder(this.options.audio_codec_name || context.codec_id);

  context.lowres = Math.min(this.options.lowres, codec.max_lowres);
  context.workaround_bugs = this.options.workaround_bugs;
  context.error_concealment = this.options.error_concealment;

  // if (context.lowres) context.flags |= CODEC_FLAG_EMU_EDGE;
  // if (this.options.fast) context.flags2 |= CODEC_FLAG2_FAST;

  var option = {threads: 'auto'};
  if (context.lowres) option.lowres = context.lowres;
  if (!codec || context.open(codec, option) < 0)
    return;

  this.index = stream.index;
  this.stream = stream;
  this.context = stream.codec;
  this.stream.discard = FFmpeg.AVDISCARD_DEFAULT;
};

Subtitle.prototype.close = function () {
  this.stream.discard = FFmpeg.AVDISCARD_ALL;
  if (this.context && this.context.is_open)
    this.context.close();

  this.index = -1;
  this.stream = null;
  this.context = null;
};

Subtitle.prototype.channel = function (video_index) {
  var index = this.index;

  var program = null;
  if ((video_index || -1) >= 0)
    program = this.format.context.findProgramFromStream(null, video_index);
  if (program) {
    for (index = 0; index < program.stream_indexes.length; index += 1) {
      if (p.stream_indexes[index] == this.index)
        break;
    }
    if (index == program.stream_indexes.length)
      index = -1;
  }

  while (true) {
    index += 1;
    if (index >= this.context.streams.length) {
      index = -1;
      break;
    }
    if (index == this.index)
      return;
    var stream = this.format.context.streams[program ? program.stream_indexes[index] : index];
    if (stream.codec.codec_type == FFmpeg.AVMEDIA_TYPE_SUBTITLE)
      break;
  }

  this.close();
  if (index >= 0)
    this.open(this.format.context.streams[program ? program.stream_indexes[index] : index]);

  console.log('subtt.channel() ' + this.index + ' -> ' + index);
};

Subtitle.prototype.decode = function (packet) {
  var subtt = new FFmpeg.AVSubtitle();

  // this.context.flushBuffers();
  var ret = this.context.decodeSubtitle(subtt, packet);
  if (ret[0] < 0)
    return;

  if (ret[1]) {
    subtt.rects.forEach(function (rect) {
      for (var c = 0; c < rect.nb_colors; c++) {
        var r = rect.pict.data[1][4 * c + 0];
        var g = rect.pict.data[1][4 * c + 1];
        var b = rect.pict.data[1][4 * c + 2];
        var a = rect.pict.data[1][4 * c + 3];
      }
    });

    console.log([
      '#subtt { ',
        'pts: ', subtt.pts, ', ',
        'format: ', subtt.format, ' ',
      '}'
    ].join(''));

    subtt.free();
  }
};

module.exports = Subtitle;
