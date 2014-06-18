
FFmpeg = require './ffmpeg'

Format = require './format'
Video = require './video'
Audio = require './audio'
Subtitle = require './subtitle'

###
player = new Player window
viewer = new Viewer window

(format.into video, audio, subtt).into viewer
player.into format, video, audio, subtt, viewer

player.open 'filename', options
player.play 10
player.pause()
player.resume()
player.stop()
player.close()

player.seek pos
player.seek step
player.seek chapter
player.step()

player.screen 'full'
player.screen 'window'
player.screen 'resize'
player.screen 'x1'
player.volume 10
player.volume 'mute'
player.channel 'audio'
player.channel 'video'
player.channel 'subtitle'
player.info()
###

module.exports =
class Player
  constructor: ->
    @options =
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

    @format = new Format @options
    @video = new Video @format
    @audio = new Audio @format
    @subtt = new Subtitle @format

  open: (filename) ->
    @format.open filename

    {video_index, audio_index, subtt_index} = @format.bestStreams()
    @video.open @format.context.streams[video_index] if video_index >= 0
    @audio.open @format.context.streams[audio_index] if audio_index >= 0
    @subtt.open @format.context.streams[subtt_index] if subtt_index >= 0

    return @

  close: ->
    @video?.close()
    @audio?.close()
    @subtt?.close()
    @format?.close()

  play: (times) ->
    for i in [0...(times ? 1)]
      @format.read (ret, packet) =>
        console.log "#packet {
          stream_index: #{packet.stream_index},
          pts: #{packet.pts},
          dts: #{packet.dts},
          size: #{packet.size},
          duration: #{packet.duration},
          pos: #{packet.pos}
        }"

        switch packet.stream_index
          when @video.index then @video.decode packet
          when @audio.index then @audio.decode packet
          when @subtt.index then @subtt.decode packet
        packet.free()

    null

  toggle: ->
  stop: ->

  seek: ->
  step: ->

  screen: ->
  volume: ->
  channel: ->
