ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVFormatContext = avformat.AVFormatContext

AVCodecID = avcodec.AVCodecID
AVDiscard = avcodec.AVDiscard
AVMediaType = avutil.AVMediaType
AVPixelFormat = avutil.AVPixelFormat
AVSampleFormat = avutil.AVSampleFormat


describe 'AVStream', ->

  describe 'open AVStream', ->
    ctx = null

    beforeEach ->
      ctx = new AVFormatContext
      fmt = avformat.findInputFormat 'mp4'
      ctx.openInput './examples/mpeg4conformance.mp4', fmt

    afterEach ->
      ctx.closeInput()
      ctx = null

    it 'streams[0]', ->
      stream = ctx.streams[0]

      expect(stream.index).toBe 0
      expect(stream.id).toBe 1
      expect(stream.codec.is_open).toBe false
      expect(stream.codec.codec_type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
      expect(stream.codec.codec).toBe null
      expect(stream.codec.codec_id).toBe AVCodecID.AV_CODEC_ID_MPEG4
      expect(stream.codec.flags).toBe 0
      expect(stream.codec.flags2).toBe 0
      expect(stream.codec.time_base).toEqual num: 0, den: 1
      expect(stream.codec.width).toBe 0
      expect(stream.codec.height).toBe 0
      expect(stream.codec.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
      expect(stream.codec.sample_rate).toBe 0
      expect(stream.codec.channels).toBe 0
      expect(stream.codec.sample_fmt).toBe AVSampleFormat.AV_SAMPLE_FMT_NONE
      expect(stream.codec.channel_layout).toBe 0
      expect(stream.codec.lowres).toBe 0
      expect(stream.time_base).toEqual num: 1, den: 90000
      expect(stream.start_time).toBe avutil.AV_NOPTS_VALUE
      expect(stream.duration).toBe 4941000
      expect(stream.nb_frames).toBe 1647
      expect(stream.disposition).toBe 1
      expect(stream.discard).toBe AVDiscard.AVDISCARD_DEFAULT
      expect(stream.sample_aspect_ratio).toEqual num: 0, den: 1
      expect(stream.metadata).toEqual {
        language: 'eng'
        creation_time: '2009-01-06 21:59:04'
      }
      expect(stream.avg_frame_rate).toEqual num: 30, den: 1
      expect(stream.event_flags).toBe 0
      expect(stream.pts_wrap_bits).toBe 64
      expect(stream.first_dts).toBe avutil.AV_NOPTS_VALUE
      expect(stream.cur_dts).toBe 9223090561878065000
      expect(stream.last_IP_pts).toBe avutil.AV_NOPTS_VALUE
      expect(stream.last_IP_duration).toBe 0
      expect(stream.r_frame_rate).toEqual num: 30, den: 1
      expect(stream.recommended_encoder_configuration).toBe ''
      expect(stream.display_aspect_ratio).toEqual num: 0, den: 0

    it 'streams[1]', ->
      stream = ctx.streams[1]

      expect(stream.index).toBe 1
      expect(stream.id).toBe 2
      expect(stream.codec.codec_type).toBe AVMediaType.AVMEDIA_TYPE_DATA
      expect(stream.codec.codec_id).toBe AVCodecID.AV_CODEC_ID_NONE

    it 'streams[2]', ->
      stream = ctx.streams[2]

      expect(stream.index).toBe 2
      expect(stream.id).toBe 3
      expect(stream.codec.codec_type).toBe AVMediaType.AVMEDIA_TYPE_DATA
      expect(stream.codec.codec_id).toBe AVCodecID.AV_CODEC_ID_NONE

  describe 'create AVStream', ->
    [ctx, stream] = [null, null]

    beforeEach ->
      ctx = new AVFormatContext
      fmt = avformat.guessFormat 'mp4', '', ''
      ctx.openOutput fmt, 'mp4', 'output.mp4'
      stream = ctx.newStream avcodec.findEncoder 'mpeg4'

    afterEach ->
      ctx.closeOutput()
      [ctx, stream] = [null, null]

    it 'one stream created', ->
      expect(ctx.streams.length).toBe 1

  describe 'test property getter/setter', ->
    [ctx, stream] = [null, null]

    beforeEach ->
      ctx = new avformat.AVFormatContext
      fmt = avformat.guessFormat 'mp4', '', ''
      ctx.openOutput fmt, 'mp4', 'output.mp4'
      stream = ctx.newStream avcodec.findEncoder 'mpeg4'

    afterEach ->
      ctx.closeOutput()
      [ctx, stream] = [null, null]

    it 'test default property', ->
      expect(stream.index).toBe 0
      expect(stream.id).toBe 0
      expect(stream.codec.is_open).toBe false
      expect(stream.codec.codec_type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
      expect(stream.codec.codec_id).toBe AVCodecID.AV_CODEC_ID_MPEG4
      expect(stream.codec.flags).toBe 0
      expect(stream.codec.flags2).toBe 0
      expect(stream.codec.time_base).toEqual num: 0, den: 1
      expect(stream.codec.width).toBe 0
      expect(stream.codec.height).toBe 0
      expect(stream.codec.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
      expect(stream.codec.sample_rate).toBe 0
      expect(stream.codec.channels).toBe 0
      expect(stream.codec.sample_fmt).toBe AVSampleFormat.AV_SAMPLE_FMT_NONE
      expect(stream.codec.channel_layout).toBe 0
      expect(stream.codec.lowres).toBe 0
      expect(stream.time_base).toEqual num: 0, den: 0
      expect(stream.start_time).toBe avutil.AV_NOPTS_VALUE
      expect(stream.duration).toBe avutil.AV_NOPTS_VALUE
      expect(stream.nb_frames).toBe 0
      expect(stream.disposition).toBe 0
      expect(stream.discard).toBe AVDiscard.AVDISCARD_DEFAULT
      expect(stream.sample_aspect_ratio).toEqual num: 0, den: 1
      expect(stream.metadata).toEqual {}
      expect(stream.avg_frame_rate).toEqual num: 0, den: 0
      expect(stream.event_flags).toBe 0
      expect(stream.pts_wrap_bits).toBe 0
      expect(stream.first_dts).toBe avutil.AV_NOPTS_VALUE
      expect(stream.cur_dts).toBe 0
      expect(stream.last_IP_pts).toBe avutil.AV_NOPTS_VALUE
      expect(stream.last_IP_duration).toBe 0
      expect(stream.r_frame_rate).toEqual num: 0, den: 0
      expect(stream.recommended_encoder_configuration).toBe ''
      expect(stream.display_aspect_ratio).toEqual num: 0, den: 0

    it '.index', ->
      stream.index = 9
      expect(stream.index).toBe 9

    it '.id', ->
      stream.id = 7
      expect(stream.id).toBe 7

    it '.time_base', ->
      stream.time_base = num: 17, den: 25
      expect(stream.time_base).toEqual num: 17, den: 25

    it '.disposition', ->
      stream.disposition = 100
      expect(stream.disposition).toBe 100

    it '.discard', ->
      stream.discard = AVDiscard.AVDISCARD_ALL
      expect(stream.discard).toBe AVDiscard.AVDISCARD_ALL

    it '.sample_aspect_ratio', ->
      stream.sample_aspect_ratio = num: 19, den: 19
      expect(stream.sample_aspect_ratio).toEqual num: 19, den: 19

    it '.avg_frame_rate', ->
      stream.avg_frame_rate = num: 10, den: 1
      expect(stream.avg_frame_rate).toEqual num: 10, den: 1

    it '.r_frame_rate', ->
      stream.r_frame_rate = num: 6, den: 35
      expect(stream.r_frame_rate).toEqual num: 6, den: 35

    it '.recommended_encoder_configuration', ->
      stream.recommended_encoder_configuration = 'aaa,bbb,ccc'
      expect(stream.recommended_encoder_configuration).toBe 'aaa,bbb,ccc'

    it '.display_aspect_ratio', ->
      stream.display_aspect_ratio = num: 100, den: 10
      expect(stream.display_aspect_ratio).toEqual num: 100, den: 10
