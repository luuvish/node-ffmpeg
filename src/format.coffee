
FFmpeg = require './ffmpeg'

module.exports =
class Format
  constructor: (@options) ->
    @context = new FFmpeg.AVFormatContext

  open: (@filename) ->
    return if not @context or @context.openInput(@filename, {}) < 0

    ###
    @context.flags |= FFmpeg.AVFMT_FLAG_GENPTS if @options.genpts

    @options.seek_by_bytes =
      (@context.iformat.flags & FFmpeg.AVFMT_TS_DISCONT) and @context.iformat.name is 'ogg'

    @max_frame_duration =
      if @context.iformat.flags & FFmpeg.AVFMT_TS_DISCONT then 10.0 else 3600.0

    if not @options.window_title and @context.metadata.title
      @options.window_title = @context.metadata.title + ' - ' + @options.input_filename

    if @options.start_time isnt FFmpeg.AV_NOPTS_VALUE
      timestamp = @options.start_time
      timestamp += @context.start_time if @context.start_time isnt FFmpeg.AV_NOPTS_VALUE
      return if @context.seekFile -1, INT64_MIN, timestamp, INT64_MAX, 0 < 0
    ###

    @realtime =
      @context.iformat.name is 'rtp' or
      @context.iformat.name is 'rtsp' or
      @context.iformat.name is 'sdp' or
      @filename.match '^rtsp:' or
      @filename.match '^udp:'
    @options.infinite_buffer = 1 if @options.infinite_buffer < 0 and @realtime

    stream.discard = FFmpeg.AVDISCARD_ALL for stream in @context.streams

    @context.dumpFormat 0, filename, 0 if @options.show_status

  close: ->
    @context?.closeInput()

  bestStreams: ->
    video_index =
      unless @options.video_disable
        @context.findBestStream FFmpeg.AVMEDIA_TYPE_VIDEO,
          @options.wanted_stream.video, -1, 0
      else -1
    audio_index =
      unless @options.audio_disable
        @context.findBestStream FFmpeg.AVMEDIA_TYPE_AUDIO,
          @options.wanted_stream.audio, video_index, 0
      else -1
    subtt_index =
      unless @options.subtt_disable
        @context.findBestStream FFmpeg.AVMEDIA_TYPE_SUBTITLE,
          @options.wanted_stream.subtt, (if audio_index >= 0 then audio_index else video_index), 0
      else -1

    {video_index, audio_index, subtt_index}

  read: (packet) ->
    packet.free()

    return if @context.readFrame(packet) < 0

    console.log "#packet {
      stream_index: #{packet.stream_index},
      pts: #{packet.pts},
      dts: #{packet.dts},
      size: #{packet.size},
      duration: #{packet.duration},
      pos: #{packet.pos}
    }"
