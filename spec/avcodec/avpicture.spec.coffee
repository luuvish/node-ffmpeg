ffmpeg = require '../../lib/ffmpeg'

describe 'AVPicture', ->
  avformat = ffmpeg.avformat
  avcodec = ffmpeg.avcodec
  avutil = ffmpeg.avutil
  AVDiscard = ffmpeg.avcodec.AVDiscard
  AVPixelFormat = ffmpeg.avutil.AVPixelFormat

  ctx = new avformat.AVFormatContext
  ctx.openInput './examples/ticket-a_aac.mkv'

  stream.discard = AVDiscard.AVDISCARD_ALL for stream in ctx.streams

  stream = ctx.streams[0]
  codec = avcodec.findDecoder stream.codec.codec_id
  stream.codec.open codec
  stream.discard = AVDiscard.AVDISCARD_DEFAULT

  it 'create AVPicture', ->
    picture = new avcodec.AVPicture

    expect(picture.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
    expect(picture.data.length).toBe 8
    expect(picture.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
    expect(picture.width).toBe 0
    expect(picture.height).toBe 0

    picture.alloc AVPixelFormat.AV_PIX_FMT_YUV420P, 176, 144
    expect(picture.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
    expect(picture.data[0][25343]).not.toBe undefined
    expect(picture.data[0][25344]).toBe undefined
    expect(picture.data[1][6335]).not.toBe undefined
    expect(picture.data[1][6336]).toBe undefined
    expect(picture.data[2][6335]).not.toBe undefined
    expect(picture.data[2][6336]).toBe undefined
    expect(picture.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_YUV420P
    expect(picture.width).toBe 176
    expect(picture.height).toBe 144

    picture.free()
    expect(picture.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
    expect(picture.data.length).toBe 8
    expect(picture.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
    expect(picture.width).toBe 0
    expect(picture.height).toBe 0

  it 'AVPicture.getSize()', ->
    getSize = avcodec.AVPicture.getSize

    pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
    expect(getSize pix_fmt, 720, 480).toBe 720 * 480 * 3 / 2
    expect(getSize pix_fmt, 1920, 1080).toBe 1920 * 1080 * 3 / 2

    pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV422P
    expect(getSize pix_fmt, 720, 480).toBe 720 * 480 * 2
    expect(getSize pix_fmt, 1920, 1080).toBe 1920 * 1080 * 2

  it 'AVPicture.fill()', ->
    pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
    size = avcodec.AVPicture.getSize pix_fmt, 176, 144
    ptr = new Uint8Array size

    picture = new avcodec.AVPicture
    expect(picture.fill ptr, pix_fmt, 176, 144).toBe size
    expect(picture.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
    expect(picture.data[0][25343]).not.toBe undefined
    expect(picture.data[0][25344]).toBe undefined
    expect(picture.data[1][6335]).not.toBe undefined
    expect(picture.data[1][6336]).toBe undefined
    expect(picture.data[2][6335]).not.toBe undefined
    expect(picture.data[2][6336]).toBe undefined
    expect(picture.pix_fmt).toBe pix_fmt
    expect(picture.width).toBe 176
    expect(picture.height).toBe 144

  it 'AVPicture.layout()', ->
    pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
    size = avcodec.AVPicture.getSize pix_fmt, 176, 144
    ptr = new Uint8Array size

    picture = new avcodec.AVPicture
    picture.alloc pix_fmt, 176, 144
    expect(picture.layout pix_fmt, 176, 144, ptr).toBe size

  it 'AVPicture.copy()', ->
    packet = new avcodec.AVPacket
    ctx.readFrame packet
    frame = new avutil.AVFrame
    [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

    packet = new avcodec.AVPacket
    ctx.readFrame packet
    frame = new avutil.AVFrame
    [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

    packet = new avcodec.AVPacket
    ctx.readFrame packet
    frame = new avutil.AVFrame
    [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

    picture = new avcodec.AVPicture
    picture.alloc frame.format, frame.width, frame.height
    picture.copy frame, frame.format, frame.width, frame.height
    expect(picture.linesize).toEqual [688, 344, 344, 0, 0, 0, 0, 0]
    expect(picture.data[0][198143]).not.toBe undefined
    expect(picture.data[0][198144]).toBe undefined
    expect(picture.data[1][49535]).not.toBe undefined
    expect(picture.data[1][49536]).toBe undefined
    expect(picture.data[2][49535]).not.toBe undefined
    expect(picture.data[2][49536]).toBe undefined
    expect(picture.pix_fmt).toBe frame.format
    expect(picture.width).toBe 688
    expect(picture.height).toBe 288

    pic2 = new avcodec.AVPicture
    pic2.alloc frame.format, frame.width, frame.height
    pic2.copy picture, frame.format, frame.width, frame.height
    expect(pic2.linesize).toEqual [688, 344, 344, 0, 0, 0, 0, 0]
    expect(pic2.data[0][198143]).not.toBe undefined
    expect(pic2.data[0][198144]).toBe undefined
    expect(pic2.data[1][49535]).not.toBe undefined
    expect(pic2.data[1][49536]).toBe undefined
    expect(pic2.data[2][49535]).not.toBe undefined
    expect(pic2.data[2][49536]).toBe undefined
    expect(pic2.pix_fmt).toBe frame.format
    expect(pic2.width).toBe 688
    expect(pic2.height).toBe 288

  it 'AVPicture.crop()', ->
    picture = new avcodec.AVPicture
    picture.alloc AVPixelFormat.AV_PIX_FMT_YUV420P, 176, 144
    pic2 = new avcodec.AVPicture
    pic2.crop picture, AVPixelFormat.AV_PIX_FMT_YUV420P, 10, 10
    expect(pic2.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
    expect(pic2.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_YUV420P
    expect(pic2.width).toBe 0
    expect(pic2.height).toBe 0

  it 'AVPicture.pad()', ->
    pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
    color = [128, 128, 128]
    picture = new avcodec.AVPicture
    picture.alloc pix_fmt, 176, 144
    pic2 = new avcodec.AVPicture
    pic2.alloc pix_fmt, 176, 144
    pic2.pad picture, 176, 144, pix_fmt, 10, 10, 10, 10, color
    expect(pic2.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
    expect(pic2.pix_fmt).toBe pix_fmt
    expect(pic2.width).toBe 176
    expect(pic2.height).toBe 144

  it 'AVPicture getter/setter test', ->
    picture = new avcodec.AVPicture

    picture.pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
    expect(picture.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_YUV420P

    picture.width = 720
    expect(picture.width).toBe 720

    picture.height = 480
    expect(picture.height).toBe 480
