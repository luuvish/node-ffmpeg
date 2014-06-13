
var FFmpeg = require('./ffmpeg');

var Format = function (options) {
  this.context = new FFmpeg.AVFormatContext();
  this.options = options;
};

Format.prototype.open = function (filename) {
  if (!this.context || this.context.openInput(filename, {}) < 0)
    return;

  this.filename = filename;

  // if (this.options.genpts)
  //   this.context.flags |= FFmpeg.AVFMT_FLAG_GENPTS;
  //
  // if (this.options.seek_by_bytes < 0)
  //   this.options.seek_by_bytes = !!(this.context.iformat.flags & FFmpeg.AVFMT_TS_DISCONT) &&
  //                                tihs.context.iformat.name == 'ogg'
  //
  // this.max_frame_duration = this.context.iformat.flags & FFmpeg.AVFMT_TS_DISCONT ? 10.0 : 3600.0;
  //
  // if (!this.options.window_title && this.context.metadata.title)
  //   this.options.window_title = this.context.metadata.title + ' - ' + this.options.input_filename
  //
  // if (this.options.start_time != FFmpeg.AV_NOPTS_VALUE) {
  //   var timestamp = this.options.start_time;
  //   if (this.context.start_time != FFmpeg.AV_NOPTS_VALUE)
  //     timestamp += this.context.start_time;
  //   if (this.context.seekFile(-1, INT64_MIN, timestamp, INT64_MAX, 0) < 0)
  //     return;
  // }

  this.realtime = this.context.iformat.name == 'rtp' ||
                  this.context.iformat.name == 'rtsp' ||
                  this.context.iformat.name == 'sdp' ||
                  this.filename.match('^rtp:') ||
                  this.filename.match('^udp:')

  if (this.options.infinite_buffer < 0 && this.realtime)
    this.options.infinite_buffer = 1;

  if (this.options.show_status)
    this.context.dumpFormat(0, filename, 0);

  this.context.streams.forEach(function (stream) {
    stream.discard = FFmpeg.AVDISCARD_ALL;
  });
};

Format.prototype.close = function () {
  if (this.context)
    this.context.closeInput();
};

Format.prototype.bestStreams = function () {
  var video_index = -1;
  var audio_index = -1;
  var subtt_index = -1;

  if (!this.options.video_disable)
    video_index = this.context.findBestStream(FFmpeg.AVMEDIA_TYPE_VIDEO, this.options.wanted_stream.video, -1, 0);
  if (!this.options.audio_disable)
    audio_index = this.context.findBestStream(FFmpeg.AVMEDIA_TYPE_AUDIO, this.options.wanted_stream.audio, video_index, 0);
  if (!this.options.subtt_disable)
    subtt_index = this.context.findBestStream(FFmpeg.AVMEDIA_TYPE_SUBTITLE, this.options.wanted_stream.subtt, audio_index >= 0 ? audio_index : video_index, 0);

  return {video: video_index, audio: audio_index, subtt:subtt_index};
};

Format.prototype.read = function (packet) {
  packet.free();
  
  if (this.context.readFrame(packet) < 0)
    return;

  console.log([
    '#packet { ',
      'stream_index: ', packet.stream_index, ', ',
      'pts: ', packet.pts, ', ',
      'dts: ', packet.dts, ', ',
      'size: ', packet.size, ', ',
      'duration: ', packet.duration, ', ',
      'pos: ', packet.pos, ' ',
    '}'
  ].join(''));
};

module.exports = Format;
