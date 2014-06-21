
FFmpeg = require './ffmpeg'

Format = require './format'
Video = require './video'
Audio = require './audio'
Subtitle = require './subtitle'

module.exports =
class Player
  constructor: ->
    @options = require './options'

    @format = new Format @options
    @video = new Video @format
    @audio = new Audio @format
    @subtt = new Subtitle @format

  open: (@filename, options={}) ->
    @format.open @filename, options

    {video_index, audio_index, subtt_index} = @format.bestStreams()
    @video.open @format.context.streams[video_index] if video_index >= 0
    @audio.open @format.context.streams[audio_index] if audio_index >= 0
    @subtt.open @format.context.streams[subtt_index] if subtt_index >= 0
    null

  close: ->
    @video?.close()
    @audio?.close()
    @subtt?.close()
    @format?.close()

  play: (times) ->
    for i in [0...(times ? 1)]
      @format.read (done, packet) =>
        console.log "#packet {
          stream_index: #{packet.stream_index},
          pts: #{packet.pts},
          dts: #{packet.dts},
          size: #{packet.size},
          duration: #{packet.duration},
          pos: #{packet.pos}
        }"

        switch packet.stream_index
          when @video.index then @video.decode done, packet, @video.render
          when @audio.index then @audio.decode done, packet, @audio.render
          when @subtt.index then @subtt.decode done, packet, @subtt.render
    null

  toggle: ->
  stop: ->

  seek: ({step, position, chapter}) ->
    return if pos?
    return if step?
    return if chapter?

  step: ->

  screen: (mode) ->
    return if mode is 'full'
    return if mode is 'window'
    return if mode is 'resize'
    return if mode in ['/4', '/2', 'x1', 'x2', 'x4']

  volume: (mode) ->
    return if mode is 'mute'
    return if mode is 'unmute'
    return if typeof mode is 'number'

  channel: (mode) ->
    switch mode
      when 'video' then @video.channel()
      when 'audio' then @audio.channel()
      when 'subtitle' then @subtt.channel()
      else stream.channel @video.index for stream in [@video, @audio, @subtt]

  info: ->
    @format.context.dumpFormat 0, @filename, 0
