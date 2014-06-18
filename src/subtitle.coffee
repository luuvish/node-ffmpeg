
FFmpeg = require './ffmpeg'

module.exports =
class Subtitle
  constructor: (@format) ->
    @options = @format.options

    @index = -1
    @stream = null
    @context = null

  open: (stream) ->
    context = stream.codec
    codec = FFmpeg.AVCodec.findDecoder(@options.subtt_codec_name or context.codec_id)

    context.lowres = Math.min @options.lowres, codec.max_lowres
    context.workaround_bugs = @options.workaround_bugs
    context.error_concealment = @options.error_concealment

    context.flags |= FFmpeg.CODEC_FLAG_EMU_EDGE if context.lowres > 0
    context.flags |= FFmpeg.CODEC_FLAG_EMU_EDGE if codec.capabilities & FFmpeg.CODEC_CAP_DR1
    context.flags2 |= FFmpeg.CODEC_FLAG2_FAST if @options.fast

    options = threads: 'auto'
    options.lowres = context.lowres if context.lowres > 0
    return if not codec or context.open(codec, options) < 0

    @index = stream.index
    @stream = stream
    @context = stream.codec
    @stream.discard = FFmpeg.AVDISCARD_DEFAULT

  close: ->
    @stream.discard = FFmpeg.AVDISCARD_ALL
    @context.close() if @context?.is_open

    @index = -1
    @stream = null
    @context = null

  channel: (video_index) ->
    index = @index

    program = if video_index? then @format.context.findProgramFromStream(null, video_index) else null
    if program
      for index in [0...program.stream_indexes.length]
        break if program.stream_indexes[index] is @index
      index = -1 if index is program.stream_indexes.length

    while true
      index += 1
      if index >= @context.streams.length
        index = -1
        break
      return if index is @index
      stream = @format.context.streams[if program? then program.stream_indexes[index] else index]
      break if stream.codec.codec_type is FFmpeg.AVMEDIA_TYPE_SUBTITLE

    @close()
    @open @format.context.streams[if program? then program.stream_indexes[index] else index] if index >= 0

    console.log "subtt.channel() #{@index} -> #{index}"

  decode: (packet, callback) ->
    subtt = new FFmpeg.AVSubtitle

    # @context.flushBuffers()
    if typeof callback is 'function'
      @context.decodeSubtitle subtt, packet, callback
    else
      [ret, got] = @context.decodeSubtitle subtt, packet
      [ret, got, subtt, packet]
