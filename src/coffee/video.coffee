
ffmpeg = require './ffmpeg'
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil
AVDiscard = avcodec.AVDiscard

module.exports =
class Video
  constructor: (@format) ->
    @frames = (new avutil.AVFrame for i in [0...5])

    @options = @format.options

    @index = -1
    @stream = null
    @context = null

  open: (stream) ->
    context = stream.codec
    codec = avcodec.findDecoder(@options.video_codec_name or context.codec_id)

    context.codec_id = codec.id
    context.lowres = Math.min @options.lowres, codec.max_lowres

    context.flags |= avcodec.CODEC_FLAG_EMU_EDGE if context.lowres > 0
    context.flags |= avcodec.CODEC_FLAG_EMU_EDGE if codec.capabilities & avcodec.CODEC_CAP_DR1
    context.flags2 |= avcodec.CODEC_FLAG2_FAST if @options.fast

    options = threads: 'auto', refcounted_frames: 1
    options.lowres = context.lowres if context.lowres > 0
    return if not codec or context.open(codec, options) < 0

    @index = stream.index
    @stream = stream
    @context = stream.codec
    @stream.discard = AVDiscard.AVDISCARD_DEFAULT

  close: ->
    @stream?.discard = AVDiscard.AVDISCARD_ALL
    @context.close() if @context?.is_open

    @index = -1
    @stream = null
    @context = null

  channel: ->
    length = @format.context.streams.length
    index = start = @index

    while true
      index += 1
      if index >= length
        return if start is -1
        index = 0
      return if index is start
      stream = @format.context.streams[index]
      break if stream.codec.codec_type is FFmpeg.AVMEDIA_TYPE_VIDEO

    @close()
    @open(stream) if stream?

    console.log "video.channel() #{start} -> #{@index}"

  getFrame: ->
    frame = @frames.shift()
    frame?.unref()
    frame

  putFrame: (frames...) ->
    frame?.unref() for frame in frames
    @frames.push frames...

  decode: (donePacket, packet, args) ->
    time_base = @stream.time_base
    frame_rate = @format.context.guessFrameRate @stream

    unless typeof args is 'function'
      frame = args
      [ret, got] = @context.decodeVideo frame, packet
      return [ret, got, frame, packet]

    callback = args
    frame = @getFrame()

    return setTimeout (=> @decode donePacket, packet, callback), 30 unless frame?

    doneFrame = => @putFrame frame

    # @context.flushBuffers()
    @context.decodeVideo frame, packet, (ret, got, frame, packet) ->
      donePacket()
      if ret >= 0 and got then callback doneFrame, frame else doneFrame()

  render: (done, frame) =>
    frame.pts =
      if @options.decoder_reorder_pts is -1 then frame.best_effort_timestamp
      else if @options.decoder_reorder_pts then frame.pkt_pts
      else frame.pkt_dts
    frame.sample_aspect_ratio = @format.context.guessSampleAspectRatio @stream, frame

    frame.data[2]
    frame.linesize[2]

    if @profiles?
      now = Date.now()
      diff = (now - @profiles.now) / 1000
      @profiles.fps = (@profiles.num + 1) / (@profiles.num / @profiles.fps + diff)
      @profiles.now = now
      @profiles.num += 1
    @profiles ?= now: Date.now(), num: 0, fps: 1.0

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
    } fps: #{@profiles.fps} fps"

    done()
