
ffmpeg = require './ffmpeg'
avcodec = ffmpeg.avcodec
AVDiscard = avcodec.AVDiscard

module.exports =
class Subtitle
  constructor: (@format) ->
    @subtitles = (new avcodec.AVSubtitle for i in [0...5])

    @options = @format.options

    @index = -1
    @stream = null
    @context = null

  open: (stream) ->
    context = stream.codec
    codec = avcodec.findDecoder(@options.subtt_codec_name or context.codec_id)

    context.codec_id = codec.id
    context.lowres = Math.min @options.lowres, codec.max_lowres

    context.flags |= avcodec.CODEC_FLAG_EMU_EDGE if context.lowres > 0
    context.flags |= avcodec.CODEC_FLAG_EMU_EDGE if codec.capabilities & avcodec.CODEC_CAP_DR1
    context.flags2 |= avcodec.CODEC_FLAG2_FAST if @options.fast

    options = threads: 'auto'
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
        index = -1
        break
      return if index is start
      stream = @format.context.streams[streamIndex index]
      break if stream.codec.codec_type is FFmpeg.AVMEDIA_TYPE_SUBTITLE

    @close()
    @open(stream) if stream?

    console.log "subtt.channel() #{start} -> #{@index}"

  getSubtitle: ->
    subtitle = @subtitles.shift()
    subtitle?.free()
    subtitle

  putSubtitle: (subtitles...) ->
    subtitle?.free() for subtitle in subtitles
    @subtitles.push subtitles...

  decode: (donePacket, packet, args) ->
    unless typeof args is 'function'
      subtt = args
      [ret, got] = @context.decodeSubtitle subtt, packet
      return [ret, got, subtt, packet]

    callback = args
    subtt = @getSubtitle()

    return setTimeout (=> @decode donePacket, packet, callback), 30 unless subtt?

    doneSubtitle = => @putSubtitle subtt

    # @context.flushBuffers()
    @context.decodeSubtitle subtt, packet, (ret, got, subtt, packet) ->
      doneSubtitle()
      if ret >= 0 and got then callback doneSubtitle, subtt else doneSubtitle()

  render: (done, subtt) ->
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

    done()
