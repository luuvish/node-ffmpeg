ffmpeg = require '../../lib/ffmpeg'

describe 'AVPacket', ->
  avformat = ffmpeg.avformat
  avcodec = ffmpeg.avcodec
  avutil = ffmpeg.avutil
  AVDiscard = ffmpeg.avcodec.AVDiscard
  AVSubtitleType = ffmpeg.avcodec.AVSubtitleType
  AVPixelFormat = ffmpeg.avutil.AVPixelFormat

  ctx = new avformat.AVFormatContext
  ctx.openInput './examples/h264-TTA.mkv'

  stream.discard = AVDiscard.AVDISCARD_ALL for stream in ctx.streams

  stream = ctx.streams[2]
  codec = avcodec.findDecoder stream.codec.codec_id
  stream.codec.open codec
  stream.discard = AVDiscard.AVDISCARD_DEFAULT

  it 'create AVSubtitle', ->
    subtitle = new avcodec.AVSubtitle

    expect(subtitle.format).toBe AVSubtitleType.SUBTITLE_NONE
    expect(subtitle.start_display_time).toBe 0
    expect(subtitle.end_display_time).toBe 0
    expect(subtitle.rects).toEqual []
    expect(subtitle.pts).toBe 0

    subtitle.free()

  it 'read AVSubtitle', ->
    packet = new avcodec.AVPacket
    ctx.readFrame packet

    expect(packet.pts).toBe 2070
    expect(packet.dts).toBe 2070
    expect(packet.data[0]).toBe 55
    expect(packet.data[1]).toBe 51
    expect(packet.data[96]).toBe 152
    expect(packet.data[97]).toBe 134
    expect(packet.size).toBe 98
    expect(packet.stream_index).toBe 2
    expect(packet.flags).toBe 1
    expect(packet.duration).toBe 1200
    expect(packet.pos).toBe 469421
    expect(packet.convergence_duration).toBe 0

    subtitle = new avcodec.AVSubtitle
    [ret, got_sub_ptr] = stream.codec.decodeSubtitle subtitle, packet

    expect(ret).toBe 98
    expect(got_sub_ptr).toBe true

    expect(subtitle.format).toBe 1
    expect(subtitle.start_display_time).toBe 0
    expect(subtitle.end_display_time).toBe 1200
    expect(subtitle.rects[0].x).toBe 0
    expect(subtitle.rects[0].y).toBe 0
    expect(subtitle.rects[0].w).toBe 0
    expect(subtitle.rects[0].h).toBe 0
    expect(subtitle.rects[0].nb_colors).toBe 0
    expect(subtitle.rects[0].pict.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
    expect(subtitle.rects[0].pict.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
    expect(subtitle.rects[0].pict.width).toBe 0
    expect(subtitle.rects[0].pict.height).toBe 0
    expect(subtitle.rects[0].type).toBe AVSubtitleType.SUBTITLE_ASS
    expect(subtitle.rects[0].text).toBe ''
    expect(subtitle.rects[0].ass).toMatch /^Dialogue: /
    expect(subtitle.rects[0].flags).toBe 0
    expect(subtitle.pts).toBe 2070000

    packet = new avcodec.AVPacket
    ctx.readFrame packet

    expect(packet.pts).toBe 2070
    expect(packet.dts).toBe 2070
    expect(packet.data[0]).toBe 55
    expect(packet.data[1]).toBe 56
    expect(packet.data[133]).toBe 114
    expect(packet.data[134]).toBe 125
    expect(packet.size).toBe 135
    expect(packet.stream_index).toBe 2
    expect(packet.flags).toBe 1
    expect(packet.duration).toBe 1200
    expect(packet.pos).toBe 469533
    expect(packet.convergence_duration).toBe 0

    subtitle = new avcodec.AVSubtitle
    [ret, got_sub_ptr] = stream.codec.decodeSubtitle subtitle, packet

    expect(ret).toBe 135
    expect(got_sub_ptr).toBe true

    expect(subtitle.format).toBe 1
    expect(subtitle.start_display_time).toBe 0
    expect(subtitle.end_display_time).toBe 1200
    expect(subtitle.rects[0].x).toBe 0
    expect(subtitle.rects[0].y).toBe 0
    expect(subtitle.rects[0].w).toBe 0
    expect(subtitle.rects[0].h).toBe 0
    expect(subtitle.rects[0].nb_colors).toBe 0
    expect(subtitle.rects[0].pict.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
    expect(subtitle.rects[0].pict.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
    expect(subtitle.rects[0].pict.width).toBe 0
    expect(subtitle.rects[0].pict.height).toBe 0
    expect(subtitle.rects[0].type).toBe AVSubtitleType.SUBTITLE_ASS
    expect(subtitle.rects[0].text).toBe ''
    expect(subtitle.rects[0].ass).toMatch /^Dialogue: /
    expect(subtitle.rects[0].flags).toBe 0
    expect(subtitle.pts).toBe 2070000

  it 'AVSubtitle getter/setter test', ->
    subtitle = new avcodec.AVSubtitle

    subtitle.format = AVSubtitleType.SUBTITLE_ASS
    expect(subtitle.format).toBe AVSubtitleType.SUBTITLE_ASS

    subtitle.start_display_time = 20
    expect(subtitle.start_display_time).toBe 20

    subtitle.end_display_time = 40
    expect(subtitle.end_display_time).toBe 40

    subtitle.pts = 200
    expect(subtitle.pts).toBe 200
