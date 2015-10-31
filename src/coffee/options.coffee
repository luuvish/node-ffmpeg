
ffmpeg = require './ffmpeg'
avutil = ffmpeg.avutil

module.exports =
  screen:
    full: {width: 0, height: 0}
    normal: {width: 640, height: 480}
    current: {width: 0, height: 0}
  screen_width: 0
  screen_height: 0
  video_size:
    width: 0
    height: 0
  is_full_screen: false
  audio_disable: false
  video_disable: false
  subtt_disable: false
  wanted_stream_spec:
    audio: -1
    video: -1
    subtt: -1
  start_time: avutil.AV_NOPTS_VALUE
  duration: avutil.AV_NOPTS_VALUE
  seek_by_bytes: -1
  display_disable: false
  file_iformat: null
  pixel_format: ''
  show_status: true
  fast: false
  genpts: false
  decoder_reorder_pts: -1
  lowres: 0
  av_sync_type: 'audio' #'video/ext'
  autoexit: false
  exit_on_keydown: false
  exit_on_mousedown: false
  loop: 1
  framedrop: -1
  infinite_buffer: -1
  window_title: ''
  vfilters: []
  afilters: ''
  rdftspeed: 0.02
  show_mode: 'none' #'video/audio/rdft'
  input_filename: ''
  audio_codec_name: ''
  subtt_codec_name: ''
  video_codec_name: ''
  autorotate: true
