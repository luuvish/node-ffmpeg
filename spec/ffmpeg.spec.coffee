FFmpeg = require '../lib/ffmpeg'

describe 'Test', ->
  it 'return true ..', ->
    expect(true).toBe true

describe 'Contants', ->
  it 'FFmpeg.AVFMT_NOFILE is 1', ->
    expect(FFmpeg.AVFMT_NOFILE).toBe 1

  it 'is encoder?', ->
    codec = FFmpeg.AVCodec.findDecoder 'h264'
    expect(codec.is_encoder).not.toBe true
    expect(codec.is_decoder).toBe true
    expect(codec.name).toBe 'h264'
    expect(codec.type).toBe FFmpeg.AVMEDIA_TYPE_VIDEO
    expect(codec.id).toBe 28
    expect(codec.name).toBe FFmpeg.AVCodec.getName codec.id
    expect(codec.type).toBe FFmpeg.AVCodec.getType codec.id
