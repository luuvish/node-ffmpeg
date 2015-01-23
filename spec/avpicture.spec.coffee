ffmpeg = require '../lib/ffmpeg'

describe 'AVPicture', ->
  AVPicture = ffmpeg.AVPicture
  AVPixelFormat = ffmpeg.AVPixelFormat

  it 'AVPicture.getSize() test', ->
    pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
    expect(AVPicture.getSize pix_fmt, 720, 480).toBe(720 * 480 * 3 / 2)
    expect(AVPicture.getSize pix_fmt, 1920, 1080).toBe(1920 * 1080 * 3 / 2)

    pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV422P
    expect(AVPicture.getSize pix_fmt, 720, 480).toBe(720 * 480 * 2)
    expect(AVPicture.getSize pix_fmt, 1920, 1080).toBe(1920 * 1080 * 2)

  it 'AVPicture.copy() test', ->
  it 'AVPicture.crop() test', ->
  it 'AVPicture.pad() test', ->
