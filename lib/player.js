
var FFmpeg = require('./ffmpeg');

var Format = require('./format.js');
var Video = require('./video.js');
var Audio = require('./audio.js');
var Subtitle = require('./subtitle.js');

/*
  var player = new Player(window);
  var render = new Render(window);

  format.into(audio, video, subtt).into(render);
  player.into(format, audio, video, subtt, render)

  player.open('filename', options);
  player.play();
  player.pause();
  player.resume();
  player.stop();
  player.close();

  player.seek(pos);
  player.seek(step);
  player.seek(chapter);
  player.step();

  player.screen('full');
  player.screen('window');
  player.screen('resize');
  player.screen('x1');
  player.volume(10);
  player.volume('mute');
  player.channel('audio');
  player.channel('video');
  player.channel('subtitle');
  player.info();
 */

var Player = function () {
  this.options = {
    iformat: null,
    filename: '',
    window_title: '',
    screen: {
      full: { width: 0, height: 0 },
      normal: { width: 640, height: 480 },
      current: { width: 0, height: 0 }
    },
    audio_disable: false,
    video_disable: false,
    subtt_disable: false,
    wanted_stream: {
      audio: -1,
      video: -1,
      subtt: -1
    },
    seek_by_bytes: -1,
    display_disable: false,
    show_status: true,
    av_sync_type: 'audio',
    start_time: FFmpeg.AV_NOPTS_VALUE,
    duration: FFmpeg.AV_NOPTS_VALUE,
    workaround_bugs: 1,
    fast: false,
    lowres: 0,
    error_concealment: 3,
    decoder_reorder_pts: -1,
    autoexit: false,
    exit_on_keydown: false,
    exit_on_mousedown: false,
    loop: 1,
    framedrop: -1,
    infinite_buffer: -1,
    show_mode: 'none',
    audio_codec_name: '',
    subtt_codec_name: '',
    video_codec_name: '',
    rdft_speed: 0.02,
    vfilters: '',
    afilters: ''
  };

  this.format = new Format(this.options);
  this.video = new Video(this.format);
  this.audio = new Audio(this.format);
  this.subtt = new Subtitle(this.format);
};

Player.prototype.open = function (filename) {
  var index = -1;

  this.format.open(filename);

  var indexes = this.format.bestStreams();
  if (indexes.video >= 0)
    this.video.open(this.format.context.streams[indexes.video]);
  if (indexes.audio >= 0)
    this.audio.open(this.format.context.streams[indexes.audio]);
  if (indexes.subtt >= 0)
    this.subtt.open(this.format.context.streams[indexes.subtt]);
};

Player.prototype.play = function (times) {
  var packet = new FFmpeg.AVPacket();

  times = times || 1;
  for (var i = 0; i < times; i++) {
    this.format.read(packet);
    if (this.video.index == packet.stream_index) this.video.decode(packet);
    if (this.audio.index == packet.stream_index) this.audio.decode(packet);
    if (this.subtt.index == packet.stream_index) this.subtt.decode(packet);
    packet.free();
  }
};

Player.prototype.toggle = function (option) {

};

Player.prototype.stop = function () {

};

Player.prototype.close = function () {
  this.video && this.video.close();
  this.audio && this.audio.close();
  this.subtt && this.subtt.close();
  this.format && this.format.close();
};

Player.prototype.seek = function (option) {

};

Player.prototype.step = function (option) {

};

Player.prototype.screen = function (option) {

};

Player.prototype.volume = function (option) {

};

Player.prototype.channel = function (option) {

};

module.exports = Player;
