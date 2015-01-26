ffmpeg = require '../lib/ffmpeg'

describe 'AVOutputFormat', ->
  AVOutputFormat = ffmpeg.avformat.AVOutputFormat
  AVMediaType = ffmpeg.avutil.AVMediaType

  it 'guess webm', ->
    fmt = AVOutputFormat.guessFormat 'webm', 'WebM', 'video/webm'

    expect(fmt.name).toBe 'webm'
    expect(fmt.long_name).toBe 'WebM'
    expect(fmt.mime_type).toBe 'video/webm'
    expect(fmt.extensions).toBe 'webm'
    expect(fmt.video_codec).toBe 141
    expect(fmt.audio_codec).toBe 86021
    expect(fmt.subtitle_codec).toBe 1465275476

  it 'guess vc1', ->
    fmt = AVOutputFormat.guessFormat 'vc1', '', ''

    expect(fmt.name).toBe 'vc1'
    expect(fmt.long_name).toBe 'raw VC-1 video'
    expect(fmt.mime_type).toBe ''
    expect(fmt.extensions).toBe 'vc1'
    expect(fmt.video_codec).toBe 71
    expect(fmt.audio_codec).toBe 0
    expect(fmt.subtitle_codec).toBe 0

  it 'guessCodec(), queryCodec()', ->
    fmt = AVOutputFormat.guessFormat 'vc1', '', ''
    guessCodec = AVOutputFormat.guessCodec
    queryCodec = AVOutputFormat.queryCodec

    expect(guessCodec fmt, 'vc1', '', '', 0).toBe 71
    expect(queryCodec fmt, 71, AVMediaType.AVMEDIA_TYPE_VIDEO).toBe 1
