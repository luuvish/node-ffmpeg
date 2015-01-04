FFmpeg = require '../lib/ffmpeg'
'''
describe 'Test', ->
  AVInputFormat = FFmpeg.AVInputFormat

  it '', ->
    mp4 = AVInputFormat.findInputFormat 'mp4'

    expect(mp4.name).toBe 'mov,mp4,m4a,3gp,3g2,mj2'
    expect(mp4.long_name).toBe 'QuickTime / MOV'

  it '', ->
    h264 = AVInputFormat.findInputFormat 'h264'

    expect(h264.name).toBe 'h264'
    expect(h264.long_name).toBe 'raw H.264 video'


describe 'Test', ->
  AVOutputFormat = FFmpeg.AVOutputFormat

  it '', ->
    webm = AVOutputFormat.guessFormat '', '', 'webm'

    expect(webm.name).toBe 'webm'
    expect(webm.long_name).toBe 'WebM'
    expect(webm.mine_type).toBe 'video/webm'
    expect(webm.extensions).toBe 'webm'

  it '', ->
    vc1 = AVOutputFormat.guessFormat '', '', 'vc1'

    expect(vc1.name).toBe 'vc1'
    expect(vc1.long_name).toBe 'raw VC-1 video'
    expect(vc1.mine_type).toBe 'video/vc1'
    expect(vc1.extensions).toBe 'vc1'

  it '', ->
    expect(AVOutputFormat.guessCodec vc1, '', '', '', 0).toBe 71
    AVOutputFormat.queryCodec vc1, 71, 0
'''