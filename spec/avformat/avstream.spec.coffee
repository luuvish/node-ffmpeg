ffmpeg = require '../../lib/ffmpeg'

describe 'AVStream', ->
  avformat = ffmpeg.avformat
  avutil = ffmpeg.avutil
  AVCodecID = ffmpeg.avcodec.AVCodecID
  AVDiscard = ffmpeg.avcodec.AVDiscard
  AVMediaType = ffmpeg.avutil.AVMediaType
  AVPixelFormat = ffmpeg.avutil.AVPixelFormat
  AVSampleFormat = ffmpeg.avutil.AVSampleFormat

  it 'input AVStream', ->
    ctx = new avformat.AVFormatContext
    fmt = avformat.findInputFormat 'mp4'
    ctx.openInput './examples/mpeg4conformance.mp4', fmt

    expect(ctx.streams[0].index).toBe 0
    expect(ctx.streams[0].id).toBe 1
    expect(ctx.streams[0].codec.is_open).toBe false
    expect(ctx.streams[0].codec.codec_type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
    expect(ctx.streams[0].codec.codec).toBe null
    expect(ctx.streams[0].codec.codec_id).toBe AVCodecID.AV_CODEC_ID_MPEG4
    expect(ctx.streams[0].codec.flags).toBe 0
    expect(ctx.streams[0].codec.flags2).toBe 0
    expect(ctx.streams[0].codec.time_base).toEqual {num: 0, den: 1}
    expect(ctx.streams[0].codec.width).toBe 0
    expect(ctx.streams[0].codec.height).toBe 0
    expect(ctx.streams[0].codec.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
    expect(ctx.streams[0].codec.sample_rate).toBe 0
    expect(ctx.streams[0].codec.channels).toBe 0
    expect(ctx.streams[0].codec.sample_fmt).toBe AVSampleFormat.AV_SAMPLE_FMT_NONE
    expect(ctx.streams[0].codec.channel_layout).toBe 0
    expect(ctx.streams[0].codec.lowres).toBe 0
    expect(ctx.streams[0].time_base).toEqual {num: 1, den: 90000}
    expect(ctx.streams[0].start_time).toBe avutil.AV_NOPTS_VALUE
    expect(ctx.streams[0].duration).toBe 4941000
    expect(ctx.streams[0].nb_frames).toBe 1647
    expect(ctx.streams[0].disposition).toBe 1
    expect(ctx.streams[0].discard).toBe AVDiscard.AVDISCARD_DEFAULT
    expect(ctx.streams[0].sample_aspect_ratio).toEqual {num: 0, den: 1}
    expect(ctx.streams[0].metadata).toEqual {
      language: 'eng'
      creation_time: '2009-01-06 21:59:04'
    }
    expect(ctx.streams[0].avg_frame_rate).toEqual {num: 30, den: 1}
    expect(ctx.streams[0].event_flags).toBe 0
    expect(ctx.streams[0].pts_wrap_bits).toBe 64
    expect(ctx.streams[0].first_dts).toBe avutil.AV_NOPTS_VALUE
    expect(ctx.streams[0].cur_dts).toBe 9223090561878065000
    expect(ctx.streams[0].last_IP_pts).toBe avutil.AV_NOPTS_VALUE
    expect(ctx.streams[0].last_IP_duration).toBe 0
    expect(ctx.streams[0].r_frame_rate).toEqual {num: 30, den: 1}
    expect(ctx.streams[0].recommended_encoder_configuration).toBe ''
    expect(ctx.streams[0].display_aspect_ratio).toEqual {num: 0, den: 0}

    expect(ctx.streams[1].index).toBe 1
    expect(ctx.streams[1].id).toBe 2
    expect(ctx.streams[1].codec.codec_type).toBe AVMediaType.AVMEDIA_TYPE_DATA
    expect(ctx.streams[1].codec.codec_id).toBe AVCodecID.AV_CODEC_ID_NONE

    expect(ctx.streams[2].index).toBe 2
    expect(ctx.streams[2].id).toBe 3
    expect(ctx.streams[2].codec.codec_type).toBe AVMediaType.AVMEDIA_TYPE_DATA
    expect(ctx.streams[2].codec.codec_id).toBe AVCodecID.AV_CODEC_ID_NONE

    ctx.closeInput()
    ctx = null

  it 'output AVStream', ->
    ctx = new avformat.AVFormatContext
    fmt = avformat.guessFormat 'mp4', '', ''
    ctx.openOutput fmt, 'mp4', 'output.mp4'

    stream = ctx.newStream ffmpeg.avcodec.findEncoder 'mpeg4'

    expect(ctx.streams.length).toBe 1

    expect(stream.index).toBe 0
    expect(stream.id).toBe 0
    expect(stream.codec.is_open).toBe false
    expect(stream.codec.codec_type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
    expect(stream.codec.codec_id).toBe AVCodecID.AV_CODEC_ID_MPEG4
    expect(stream.codec.flags).toBe 0
    expect(stream.codec.flags2).toBe 0
    expect(stream.codec.time_base).toEqual {num: 0, den: 1}
    expect(stream.codec.width).toBe 0
    expect(stream.codec.height).toBe 0
    expect(stream.codec.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
    expect(stream.codec.sample_rate).toBe 0
    expect(stream.codec.channels).toBe 0
    expect(stream.codec.sample_fmt).toBe AVSampleFormat.AV_SAMPLE_FMT_NONE
    expect(stream.codec.channel_layout).toBe 0
    expect(stream.codec.lowres).toBe 0
    expect(stream.time_base).toEqual {num: 0, den: 0}
    expect(stream.start_time).toBe avutil.AV_NOPTS_VALUE
    expect(stream.duration).toBe avutil.AV_NOPTS_VALUE
    expect(stream.nb_frames).toBe 0
    expect(stream.disposition).toBe 0
    expect(stream.discard).toBe AVDiscard.AVDISCARD_DEFAULT
    expect(stream.sample_aspect_ratio).toEqual {num: 0, den: 1}
    expect(stream.metadata).toEqual {}
    expect(stream.avg_frame_rate).toEqual {num: 0, den: 0}
    expect(stream.event_flags).toBe 0
    expect(stream.pts_wrap_bits).toBe 0
    expect(stream.first_dts).toBe avutil.AV_NOPTS_VALUE
    expect(stream.cur_dts).toBe 0
    expect(stream.last_IP_pts).toBe avutil.AV_NOPTS_VALUE
    expect(stream.last_IP_duration).toBe 0
    expect(stream.r_frame_rate).toEqual {num: 0, den: 0}
    expect(stream.recommended_encoder_configuration).toBe ''
    expect(stream.display_aspect_ratio).toEqual {num: 0, den: 0}

    ctx.closeOutput()
    ctx = null

  it 'AVStream getter/setter test', ->
    ctx = new avformat.AVFormatContext
    fmt = avformat.guessFormat 'mp4', '', ''
    ctx.openOutput fmt, 'mp4', 'output.mp4'

    stream = ctx.newStream ffmpeg.avcodec.findEncoder 'mpeg4'

    stream.index = 9
    expect(stream.index).toBe 9

    stream.id = 7
    expect(stream.id).toBe 7

    stream.time_base = {num: 17, den: 25}
    expect(stream.time_base).toEqual {num: 17, den: 25}

    stream.disposition = 100
    expect(stream.disposition).toBe 100

    stream.discard = AVDiscard.AVDISCARD_ALL
    expect(stream.discard).toBe AVDiscard.AVDISCARD_ALL

    stream.sample_aspect_ratio = {num: 19, den: 19}
    expect(stream.sample_aspect_ratio).toEqual {num: 19, den: 19}

    stream.avg_frame_rate = {num: 10, den: 1}
    expect(stream.avg_frame_rate).toEqual {num: 10, den: 1}

    stream.r_frame_rate = {num: 6, den: 35}
    expect(stream.r_frame_rate).toEqual {num: 6, den: 35}

    stream.recommended_encoder_configuration = 'aaa,bbb,ccc'
    expect(stream.recommended_encoder_configuration).toBe 'aaa,bbb,ccc'

    stream.display_aspect_ratio = {num: 100, den: 10}
    expect(stream.display_aspect_ratio).toEqual {num: 100, den: 10}

    ctx.closeOutput()
    ctx = null
