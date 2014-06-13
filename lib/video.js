
var FFmpeg = require('./ffmpeg');

var Video = function (format) {
  this.format = format;
  this.options = format.options;

  this.index = -1;
  this.stream = null;
  this.context = null;
};

Video.prototype.open = function (stream) {
  var context = stream.codec;
  var codec = FFmpeg.AVCodec.findDecoder(this.options.video_codec_name || context.codec_id);

  context.lowres = Math.min(this.options.lowres, codec.max_lowres);
  context.workaround_bugs = this.options.workaround_bugs;
  context.error_concealment = this.options.error_concealment;

  // if (context.lowres) context.flags |= CODEC_FLAG_EMU_EDGE;
  // if (this.options.fast) context.flags2 |= CODEC_FLAG2_FAST;

  var option = {threads: 'auto', refcounted_frames: 1};
  if (context.lowres) option.lowres = context.lowres;
  if (!codec || context.open(codec, option) < 0)
    return;

  this.index = stream.index;
  this.stream = stream;
  this.context = stream.codec;
  this.stream.discard = FFmpeg.AVDISCARD_DEFAULT;
};

Video.prototype.close = function () {
  this.stream.discard = FFmpeg.AVDISCARD_ALL;
  if (this.context && this.context.is_open)
    this.context.close();

  this.index = -1;
  this.stream = null;
  this.context = null;
};

Video.prototype.channel = function () {
  var index = this.index;

  while (true) {
    index += 1;
    if (index >= this.context.streams.length) {
      if (this.index == -1)
        return;
      index = 0;
    }
    if (index == this.index)
      return;
    if (this.format.context.streams[index].codec.codec_type == FFmpeg.AVMEDIA_TYPE_VIDEO)
      break;
  }

  this.close();
  this.open(this.format.context.streams[index]);

  console.log('video.channel() ' + this.index + ' -> ' + index);
};

Video.prototype.decode = function (packet) {
  var frame = new FFmpeg.AVFrame();

  var time_base = this.stream.time_base;
  var frame_rate = this.format.context.guessFrameRate(this.stream);

  // this.context.flushBuffers();
  var ret = this.context.decodeVideo(frame, packet);
  if (ret[0] < 0)
    return;

  if (ret[1]) {
    if (this.options.decoder_reorder_pts == -1)
      frame.pts = frame.best_effort_timestamp;
    else if (this.options.decoder_reorder_pts)
      frame.pts = frame.pkt_pts;
    else
      frame.pts = frame.pkt_dts;
    frame.sample_aspect_ratio = this.format.context.guessSampleAspectRatio(this.stream, frame);

    frame.data[2];
    frame.linesize[2];

    console.log([
      '#video { ',
        'pts: ', frame.pts, ', ',
        'sar: { ',
          'num: ', frame.sample_aspect_ratio.num, ', ',
          'den: ', frame.sample_aspect_ratio.den, ' ',
        '}, ',
        'width: ', frame.width, ', ',
        'height: ', frame.height, ', ',
        'format: ', frame.format, ', ',
        'pkt_pos: ', frame.pkt_pos, ' ',
      '}'
    ].join(''));

    frame.unref();
  }
};

module.exports = Video;
