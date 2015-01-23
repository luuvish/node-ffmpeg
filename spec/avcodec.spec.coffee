FFmpeg = require '../lib/ffmpeg'

"""
xdescribe 'AVCodec configs', ->

  it 'version', ->
    FFmpeg.AVCodec.version()
  it 'configuration', ->
    FFmpeg.AVCodec.configuration()
  it 'license', ->
    FFmpeg.AVCodec.license()


describe 'AVCodec find decoder by name', ->

  it 'find decoder mpeg2', ->
    codec = FFmpeg.AVCodec.findDecoder 'mpeg2video'

    expect(codec.is_encoder).toBe false
    expect(codec.is_decoder).toBe true

    expect(codec.name).toBe 'mpeg2video'
    expect(codec.long_name).toBe 'MPEG-2 video'
    expect(codec.type).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(codec.id).toBe 2 # AV_CODEC_ID_MPEG2VIDEO

    #expect(codec.capabilities).toBe 0
    #expect(codec.supported_framerates).toBe {num: 0, dem: 1}
    #expect(codec.pix_fmts).toBe {}
    #expect(codec.supported_samplerates).toBe {}
    #expect(codec.sample_fmts).toBe {}
    #expect(codec.channel_layouts).toBe {}
    expect(codec.max_lowres).toBe 3

  it 'find decoder mpeg4', ->
    codec = FFmpeg.AVCodec.findDecoder 'mpeg4'

    expect(codec.is_encoder).toBe false
    expect(codec.is_decoder).toBe true

    expect(codec.name).toBe 'mpeg4'
    expect(codec.long_name).toBe 'MPEG-4 part 2'
    expect(codec.type).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(codec.id).toBe 13 # AV_CODEC_ID_MPEG4

    expect(codec.max_lowres).toBe 3

  it 'find decoder h264', ->
    codec = FFmpeg.AVCodec.findDecoder 'h264'

    expect(codec.is_encoder).toBe false
    expect(codec.is_decoder).toBe true

    expect(codec.name).toBe 'h264'
    expect(codec.long_name).toBe 'H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10'
    expect(codec.type).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(codec.id).toBe 28 # AV_CODEC_ID_H264

    expect(codec.max_lowres).toBe 0

  it 'find decoder vc1', ->
    codec = FFmpeg.AVCodec.findDecoder 'vc1'

    expect(codec.is_encoder).toBe false
    expect(codec.is_decoder).toBe true

    expect(codec.name).toBe 'vc1'
    expect(codec.long_name).toBe 'SMPTE VC-1'
    expect(codec.type).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(codec.id).toBe 71 # AV_CODEC_ID_VC1

    expect(codec.max_lowres).toBe 0

  it 'find decoder hevc', ->
    codec = FFmpeg.AVCodec.findDecoder 'hevc'

    expect(codec.is_encoder).toBe false
    expect(codec.is_decoder).toBe true

    expect(codec.name).toBe 'hevc'
    expect(codec.long_name).toBe 'HEVC (High Efficiency Video Coding)'
    expect(codec.type).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(codec.id).toBe 1211250229 # AV_CODEC_ID_HEVC

    expect(codec.max_lowres).toBe 0


describe 'AVCodec find decoder by id', ->

  it 'find decoder hevc', ->
    codec = FFmpeg.AVCodec.findDecoder 1211250229

    expect(codec.is_encoder).toBe false
    expect(codec.is_decoder).toBe true

    expect(codec.name).toBe 'hevc'
    expect(codec.long_name).toBe 'HEVC (High Efficiency Video Coding)'
    expect(codec.type).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(codec.id).toBe 1211250229 # AV_CODEC_ID_HEVC

    expect(codec.max_lowres).toBe 0


describe 'AVCodec get name or type', ->

  it 'get name by codec id', ->
    expect(FFmpeg.AVCodec.getName 2).toBe 'mpeg2video'
    expect(FFmpeg.AVCodec.getName 13).toBe 'mpeg4'
    expect(FFmpeg.AVCodec.getName 28).toBe 'h264'
    expect(FFmpeg.AVCodec.getName 71).toBe 'vc1'
    expect(FFmpeg.AVCodec.getName 1211250229).toBe 'hevc'

  it 'get type by codec id', ->
    expect(FFmpeg.AVCodec.getType 2).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(FFmpeg.AVCodec.getType 13).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(FFmpeg.AVCodec.getType 28).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(FFmpeg.AVCodec.getType 71).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(FFmpeg.AVCodec.getType 1211250229).toBe FFmpeg.AVMEDIA_TYPE_VIDEO


xdescribe 'AVCodecContext', ->
  ctx = new FFmpeg.AVCodecContext()

  xit '', ->
    ctx.pkt_timebase = num: 0, dem: 1
    expect(ctx.pkt_timebase).toBe {num: 0, dem: 1}

  xit '', ->
    expect(ctx.descriptor).toBe {}

  xit '', ->
    ctx.lowres = 4
    expect(ctx.lowres).toBe 4

  xit '', ->
    ctx.seek_preroll = 4
    expect(ctx.seek_preroll).toBe 4
"""
