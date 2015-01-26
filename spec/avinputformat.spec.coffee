ffmpeg = require '../lib/ffmpeg'

describe 'AVInputFormat', ->
  AVInputFormat = ffmpeg.avformat.AVInputFormat

  it 'find mp4', ->
    fmt = AVInputFormat.findInputFormat 'mp4'

    expect(fmt.name).toBe 'mov,mp4,m4a,3gp,3g2,mj2'
    expect(fmt.long_name).toBe 'QuickTime / MOV'
    expect(fmt.flags).toBe 32768
    expect(fmt.extensions).toBe 'mov,mp4,m4a,3gp,3g2,mj2'
    expect(fmt.mime_type).toBe ''
    expect(fmt.raw_codec_id).toBe 0

  it 'find h264', ->
    fmt = AVInputFormat.findInputFormat 'h264'

    expect(fmt.name).toBe 'h264'
    expect(fmt.long_name).toBe 'raw H.264 video'
    expect(fmt.flags).toBe 256
    expect(fmt.extensions).toBe 'h26l,h264,264,avc'
    expect(fmt.mime_type).toBe ''
    expect(fmt.raw_codec_id).toBe 28

  it 'find hevc', ->
    fmt = AVInputFormat.findInputFormat 'hevc'

    expect(fmt.name).toBe 'hevc'
    expect(fmt.long_name).toBe 'raw HEVC video'
    expect(fmt.flags).toBe 256
    expect(fmt.extensions).toBe 'hevc,h265,265'
    expect(fmt.mime_type).toBe ''
    expect(fmt.raw_codec_id).toBe 1211250229
