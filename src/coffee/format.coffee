
ffmpeg = require './ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil
AVDiscard = avcodec.AVDiscard
AVMediaType = avutil.AVMediaType

module.exports =
class Format
  constructor: (@options) ->
    @context = new avformat.AVFormatContext
    @packets = (new avcodec.AVPacket for i in [0...10])

  open: (@filename, options={}) ->
    return if not @context or @context.openInput(@filename, options) < 0

    @context.flags |= avformat.AVFMT_FLAG_GENPTS if @options.genpts

    @context.injectGlobalSideData()

    opts = @context.findStreamInfo()

    @options.seek_by_bytes =
      (@context.iformat.flags & avformat.AVFMT_TS_DISCONT) and @context.iformat.name is 'ogg'

    @max_frame_duration =
      if @context.iformat.flags & avformat.AVFMT_TS_DISCONT then 10.0 else 3600.0

    if not @options.window_title and @context.metadata.title?
      @options.window_title = "#{@context.metadata.title} - #{@options.input_filename}"

    if @options.start_time isnt avutil.AV_NOPTS_VALUE
      timestamp = @options.start_time
      timestamp += @context.start_time if @context.start_time isnt avutil.AV_NOPTS_VALUE
      return if @context.seekFile(-1, ffmpeg.INT64_MIN, timestamp, ffmpeg.INT64_MAX, 0) < 0

    @realtime =
      @context.iformat.name in ['rtp', 'rtsp', 'sdp'] or
      @filename.match '^rtp:' or @filename.match '^udp:'
    @options.infinite_buffer = 1 if @options.infinite_buffer < 0 and @realtime

    stream.discard = AVDiscard.AVDISCARD_ALL for stream in @context.streams
    #ctx.matchStreamSpecifier @context.streams, ''

    @context.dumpFormat 0, @filename, 0 if @options.show_status

  close: ->
    @context?.closeInput()

  bestStreams: ->
    [video_index, video_codec] =
      unless @options.video_disable
        @context.findBestStream AVMediaType.AVMEDIA_TYPE_VIDEO,
          @options.wanted_stream.video, -1, 0
      else [-1, null]
    [audio_index, audio_codec] =
      unless @options.audio_disable
        @context.findBestStream AVMediaType.AVMEDIA_TYPE_AUDIO,
          @options.wanted_stream.audio, video_index, 0
      else [-1, null]
    [subtt_index, subtt_codec] =
      unless @options.subtt_disable
        @context.findBestStream AVMediaType.AVMEDIA_TYPE_SUBTITLE,
          @options.wanted_stream.subtt,
          (if audio_index >= 0 then audio_index else video_index), 0
      else [-1, null]

    sar = @context.guessSampleAspectRatio @context.streams[video_index]

    {video_index, audio_index, subtt_index}

  getPacket: ->
    packet = @packets.shift()
    packet?.free()
    packet

  putPacket: (packets...) ->
    packet?.free() for packet in packets
    @packets.push packets...

  read: (args) ->
    unless typeof args is 'function'
      packet = args
      ret = @context.readFrame packet
      return [ret, packet]

    callback = args
    packet = @getPacket()

    return setTimeout (=> @read callback), 30 unless packet?

    donePacket = => @putPacket packet

    @context.readFrame packet, (ret, packet) ->
      if ret >= 0 then callback donePacket, packet else donePacket()
