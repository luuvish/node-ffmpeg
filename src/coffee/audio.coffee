
ffmpeg = require './ffmpeg'
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil
AVDiscard = avcodec.AVDiscard

module.exports =
class Audio
  constructor: (@format) ->
    @frames = (new avutil.AVFrame for i in [0...5])

    @options = @format.options

    @index = -1
    @stream = null
    @context = null

  open: (stream) ->
    context = stream.codec
    codec = avcodec.findDecoder(@options.audio_codec_name or context.codec_id)

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

  channel: (video_index) ->
    length = @format.context.streams.length
    index = start = @index

    program = @format.context.findProgramFromStream(null, video_index) if video_index?
    if program
      length = program.stream_indexes.length
      for start in [0...length]
        break if program.stream_indexes[start] is @index
      start = -1 if start is length
      index = start

    streamIndex = (index) -> if program? then program.stream_indexes[index] else index

    while true
      index += 1
      if index >= length
        return if start is -1
        index = 0
      return if index is start
      stream = @format.context.streams[streamIndex index]
      break if stream.codec.codec_type is FFmpeg.AVMEDIA_TYPE_AUDIO and
               stream.codec.sample_rate isnt 0 and stream.codec.channels isnt 0

    @close()
    @open(stream) if stream?

    console.log "audio.channel() #{start} -> #{@index}"

  getFrame: ->
    frame = @frames.shift()
    frame?.unref()
    frame

  putFrame: (frames...) ->
    frame?.unref() for frame in frames
    @frames.push frames...

  decode: (donePacket, packet, args) ->
    {sample_rate, channels, channel_layout, sample_fmt} = @context

    unless typeof args is 'function'
      frame = args
      [ret, got] = @context.decodeAudio frame, packet
      return [ret, got, frame, packet]

    callback = args
    frame = @getFrame()

    return setTimeout (=> @decode donePacket, packet, callback), 30 unless frame?

    doneFrame = => @putFrame frame

    # @context.flushBuffers()
    @context.decodeAudio frame, packet, (ret, got, frame, packet) ->
      donePacket()
      if ret >= 0 and got then callback doneFrame, frame else doneFrame()

  render: (done, frame) ->
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

    done()
