
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
          when @video.index
            @video.decode packet, (ret, got, frame, packet) =>
              if ret >= 0 and got
                frame.pts =
                  if frame.pts isnt FFmpeg.AV_NOPTS_VALUE then frame.pts
                  else if frame.pkt_pts isnt FFmpeg.AV_NOPTS_VALUE then frame.pkt_pts
                  else if @frame_next_pts isnt FFmpeg.AV_NOPTS_VALUE then @frame_next_pts
                @frame_next_pts = frame.pts + frame.nb_samples if frame.pts isnt FFmpeg.AV_NOPTS_VALUE

                data = frame.data[0]
                size = FFmpeg.getSamplesBufferSize null, frame.channels, frame.nb_samples, frame.format, 1

                console.log " #audio {
                  pts: #{frame.pts},
                  format: #{frame.format},
                  channels: #{frame.channels},
                  sample_rate: #{frame.sample_rate},
                  nb_samples: #{frame.nb_samples}
                }"

                packet.free()
                frame.unref()
          when @audio.index
            @audio.decode packet, (ret, got, frame, packet) =>
              if ret >= 0 and got
                frame.pts =
                  if @options.decoder_reorder_pts is -1 then frame.best_effort_timestamp
                  else if @options.decoder_reorder_pts then frame.pkt_pts
                  else frame.pkt_dts
                frame.sample_aspect_ratio = @format.context.guessSampleAspectRatio @audio.stream, frame

                frame.data[2]
                frame.linesize[2]

                console.log " #video {
                  pts: #{frame.pts},
                  sar: {
                    num: #{frame.sample_aspect_ratio.num},
                    den: #{frame.sample_aspect_ratio.den}
                  },
                  width: #{frame.width},
                  height: #{frame.height},
                  format: #{frame.format},
                  pkt_pos: #{frame.pkt_pos}
                }"

                packet.free()
                frame.unref()
          when @subtt.index
            @subtt.decode packet, (ret, got, subtt, packet) =>
              if ret >= 0 and got
                subtt.rects.forEach (rect) ->
                  for c in [0...rect.nb_colors]
                    r = rect.pict.data[1][4 * c + 0]
                    g = rect.pict.data[1][4 * c + 0]
                    b = rect.pict.data[1][4 * c + 0]
                    a = rect.pict.data[1][4 * c + 0]

                console.log " #subtt {
                  pts: #{subtt.pts},
                  format: #{subtt.format}
                }"

                packet.free()
                subtt.free()

    null

  toggle: ->
  stop: ->

  seek: ->
  step: ->

  screen: ->
  volume: ->
  channel: ->
