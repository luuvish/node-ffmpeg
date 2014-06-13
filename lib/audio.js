
var FFmpeg = require('./ffmpeg');

var Audio = function (format) {
  this.format = format;
  this.options = format.options;

  this.index = -1;
  this.stream = null;
  this.context = null;
};

Audio.prototype.open = function (stream) {
  var context = stream.codec;
  var codec = FFmpeg.AVCodec.findDecoder(this.options.audio_codec_name || context.codec_id);

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

Audio.prototype.close = function () {
  this.stream.discard = FFmpeg.AVDISCARD_ALL;
  if (this.context && this.context.is_open)
    this.context.close();

  this.index = -1;
  this.stream = null;
  this.context = null;
};

Audio.prototype.channel = function (video_index) {
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
      if (this.index == -1)
        return;
      index = 0;
    }
    if (index == this.index)
      return;
    var stream = this.format.context.streams[program ? program.stream_indexes[index] : index];
    if (stream.codec.codec_type == FFmpeg.AVMEDIA_TYPE_AUDIO &&
        stream.codec.sample_rate != 0 && stream.codec.channels != 0)
      break;
  }

  this.close();
  this.open(this.format.context.streams[program ? program.stream_indexes[index] : index]);

  console.log('audio.channel() ' + this.index + ' -> ' + index);
};

Audio.prototype.decode = function (packet) {
  var frame = new FFmpeg.AVFrame();

  var sample_rate = this.context.sample_rate;
  var channels = this.context.channels;
  var channel_layout = this.context.channel_layout; // av_get_channel_layout_nb_channels()
  var sample_fmt = this.context.sample_fmt;

  // this.context.flushBuffers();
  var ret = this.context.decodeAudio(frame, packet);
  if (ret[0] < 0)
    return;

  if (ret[1]) {
    if (frame.pts != FFmpeg.AV_NOPTS_VALUE)
      frame.pts = frame.pts;
    else if (frame.pkt_pts != FFmpeg.AV_NOPTS_VALUE)
      frame.pts = frame.pkt_pts;
    else if (this.frame_next_pts != FFmpeg.AV_NOPTS_VALUE)
      frame.pts = this.frame_next_pts;
    if (frame.pts != FFmpeg.AV_NOPTS_VALUE)
      this.frame_next_pts = frame.pts + frame.nb_samples;

    var data = frame.data[0];
    var size = FFmpeg.getSamplesBufferSize(null, frame.channels, frame.nb_samples, frame.format , 1);

    console.log([
      '#audio { ',
        'pts: ', frame.pts, ', ',
        'format: ', frame.format, ', ',
        'channels: ', frame.channels, ', ',
        'sample_rate: ', frame.sample_rate, ', ',
        'nb_samples: ', frame.nb_samples, ' ',
      '}'
    ].join(''));

    frame.unref();
  }
};

module.exports = Audio;
