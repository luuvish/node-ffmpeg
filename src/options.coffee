
FFmpeg = require './ffmpeg'

module.exports =
  iformat: null
  filename: ''
  window_title: ''
  screen:
    full: {width: 0, height: 0}
    normal: {width: 640, height: 480}
    current: {width: 0, height: 0}
  audio_disable: false
  video_disable: false
  subtt_disable: false
  wanted_stream:
    audio: -1
    video: -1
    subtt: -1
  seek_by_bytes: -1
  display_disable: false
  show_status: true
  av_sync_type: 'audio'
  start_time: FFmpeg.AV_NOPTS_VALUE
  duration: FFmpeg.AV_NOPTS_VALUE
  workaround_bugs: 1
  fast: false
  lowres: 0
  error_concealment: 3
  decoder_reorder_pts: -1
  autoexit: false
  exit_on_keydown: false
  exit_on_mousedown: false
  loop: 1
  framedrop: -1
  infinite_buffer: -1
  show_mode: 'none'
  audio_codec_name: ''
  subtt_codec_name: ''
  video_codec_name: ''
  rdft_speed: 0.02
  vfilters: ''
  afilters: ''
