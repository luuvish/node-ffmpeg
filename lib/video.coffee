
FFmpeg = require './ffmpeg'


class Video
  constructor: (@format) ->
    @options = @format.options

    @index = -1
    @stream = null
    @context = null

  open: (stream) ->
    context = stream.codec
    codec = FFmpeg.AVCodec.findDecoder(@options.video_codec_name or context.codec_id)

    context.lowres = Math.min @options.lowres, codec.max_lowres
    context.workaround_bugs = @options.workaround_bugs
    context.error_concealment = @options.error_concealment

    # @context.flags |= CODEC_FLAG_EMU_EDGE if @context.lowres > 0
    # @context.flags2 |= CODEC_FLAG2_FAST if @options.fast

    option = threads: 'auto', refcounted_frames: 1
    option.lowres = context.lowres if context.lowres > 0
    return if not codec or context.open(codec, option) < 0

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

  channel: ->
    index = @index

    while true
      index += 1
      if index >= @context.streams.length
        return if @index is -1
        index = 0
      return if index is @index
      break if @format.context.streams[index].codec.codec_type is FFmpeg.AVMEDIA_TYPE_VIDEO

    @.close()
    @.open @format.context.streams[index]

    console.log "video.channel() #{@index} -> #{index}"

  decode: (packet) ->
    frame = new FFmpeg.AVFrame

    time_base = @stream.time_base
    frame_rate = @format.context.guessFrameRate @stream

    # @context.flushBuffers()
    [ret, got] = @context.decodeVideo frame, packet
    return if ret < 0

    if got
      frame.pts =
        if @options.decoder_reorder_pts is -1 then frame.best_effort_timestamp
        else if @options.decoder_reorder_pts then frame.pkt_pts
        else frame.pkt_dts
      frame.sample_aspect_ratio = @format.context.guessSampleAspectRatio @stream, frame

      frame.data[2]
      frame.linesize[2]

      console.log [
        "##video { "
          "pts: #{frame.pts}, "
          "sar: { "
            "num: #{frame.sample_aspect_ratio.num}, "
            "den: #{frame.sample_aspect_ratio.den} "
          "}, "
          "width: #{frame.width}, "
          "height: #{frame.height}, "
          "format: #{frame.format}, "
          "pkt_pos: #{frame.pkt_pos} "
        "}"
      ].join ''

      frame.unref()


module.exports = Video
