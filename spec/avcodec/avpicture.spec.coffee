ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVFormatContext = avformat.AVFormatContext
AVPacket = avcodec.AVPacket
AVPicture = avcodec.AVPicture
AVFrame = avutil.AVFrame

AVDiscard = avcodec.AVDiscard
AVPixelFormat = avutil.AVPixelFormat


describe 'AVPicture', ->

  describe 'test method functions', ->
    getSize = AVPicture.getSize

    [picture0, picture1] = [null, null]

    beforeEach ->
      [picture0, picture1] = [new AVPicture, new AVPicture]

    afterEach ->
      [picture0, picture1] = [null, null]

    it '.alloc()', ->
      pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
      picture0.alloc pix_fmt, 176, 144

      expect(picture0.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
      expect(picture0.data[0][25343]).not.toBeUndefined()
      expect(picture0.data[0][25344]).toBeUndefined()
      expect(picture0.data[1][6335]).not.toBeUndefined()
      expect(picture0.data[1][6336]).toBeUndefined()
      expect(picture0.data[2][6335]).not.toBeUndefined()
      expect(picture0.data[2][6336]).toBeUndefined()
      expect(picture0.pix_fmt).toBe pix_fmt
      expect(picture0.width).toBe 176
      expect(picture0.height).toBe 144

    it '.free()', ->
      pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
      picture0.alloc pix_fmt, 176, 144
      picture0.free()

      expect(picture0.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
      expect(picture0.data.length).toBe 8
      expect(picture0.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
      expect(picture0.width).toBe 0
      expect(picture0.height).toBe 0

    it '.getSize()', ->
      pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
      expect(getSize pix_fmt, 720, 480).toBe 720 * 480 * 3 / 2
      expect(getSize pix_fmt, 1920, 1080).toBe 1920 * 1080 * 3 / 2

      pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV422P
      expect(getSize pix_fmt, 720, 480).toBe 720 * 480 * 2
      expect(getSize pix_fmt, 1920, 1080).toBe 1920 * 1080 * 2

    it '.fill()', ->
      pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
      size = getSize pix_fmt, 176, 144
      ptr = new Uint8Array size

      expect(picture0.fill ptr, pix_fmt, 176, 144).toBe size

      expect(picture0.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
      expect(picture0.data[0][25343]).not.toBeUndefined()
      expect(picture0.data[0][25344]).toBeUndefined()
      expect(picture0.data[1][6335]).not.toBeUndefined()
      expect(picture0.data[1][6336]).toBeUndefined()
      expect(picture0.data[2][6335]).not.toBeUndefined()
      expect(picture0.data[2][6336]).toBeUndefined()
      expect(picture0.pix_fmt).toBe pix_fmt
      expect(picture0.width).toBe 176
      expect(picture0.height).toBe 144

    it '.layout()', ->
      pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
      size = getSize pix_fmt, 176, 144
      ptr = new Uint8Array size

      picture0.alloc pix_fmt, 176, 144

      expect(picture0.layout pix_fmt, 176, 144, ptr).toBe size

    describe '.copy()', ->
      [ctx, stream] = [null, null]
      [picture0, picture1] = [null, null]

      beforeEach ->
        ctx = new AVFormatContext
        ctx.openInput './examples/ticket-a_aac.mkv'

        stream.discard = AVDiscard.AVDISCARD_ALL for stream in ctx.streams

        stream = ctx.streams[0]
        codec = avcodec.findDecoder stream.codec.codec_id
        stream.codec.open codec
        stream.discard = AVDiscard.AVDISCARD_DEFAULT

        [picture0, picture1] = [new AVPicture, new AVPicture]

      afterEach ->
        stream.codec.close()
        ctx.closeInput()
        [ctx, stream] = [null, null]
        [picture0, picture1] = [null, null]

      it 'decoding 3 frames and copy picture', ->
        for i in [0...3]
          packet = new AVPacket
          ctx.readFrame packet
          frame = new AVFrame
          [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

        picture0.alloc frame.format, frame.width, frame.height
        picture0.copy frame, frame.format, frame.width, frame.height
        expect(picture0.linesize).toEqual [688, 344, 344, 0, 0, 0, 0, 0]
        expect(picture0.data[0][198143]).not.toBeUndefined()
        expect(picture0.data[0][198144]).toBeUndefined()
        expect(picture0.data[1][49535]).not.toBeUndefined()
        expect(picture0.data[1][49536]).toBeUndefined()
        expect(picture0.data[2][49535]).not.toBeUndefined()
        expect(picture0.data[2][49536]).toBeUndefined()
        expect(picture0.pix_fmt).toBe frame.format
        expect(picture0.width).toBe 688
        expect(picture0.height).toBe 288

        picture1.alloc frame.format, frame.width, frame.height
        picture1.copy picture0, frame.format, frame.width, frame.height
        expect(picture1.linesize).toEqual [688, 344, 344, 0, 0, 0, 0, 0]
        expect(picture1.data[0][198143]).not.toBeUndefined()
        expect(picture1.data[0][198144]).toBeUndefined()
        expect(picture1.data[1][49535]).not.toBeUndefined()
        expect(picture1.data[1][49536]).toBeUndefined()
        expect(picture1.data[2][49535]).not.toBeUndefined()
        expect(picture1.data[2][49536]).toBeUndefined()
        expect(picture1.pix_fmt).toBe frame.format
        expect(picture1.width).toBe 688
        expect(picture1.height).toBe 288

    it '.crop()', ->
      pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P

      picture0.alloc pix_fmt, 176, 144
      picture1.crop picture0, pix_fmt, 10, 10

      expect(picture1.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
      expect(picture1.pix_fmt).toBe pix_fmt
      expect(picture1.width).toBe 0
      expect(picture1.height).toBe 0

    it '.pad()', ->
      pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
      color = [128, 128, 128]

      picture0.alloc pix_fmt, 176, 144
      picture1.alloc pix_fmt, 176, 144
      picture1.pad picture0, 176, 144, pix_fmt, 10, 10, 10, 10, color

      expect(picture1.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
      expect(picture1.pix_fmt).toBe pix_fmt
      expect(picture1.width).toBe 176
      expect(picture1.height).toBe 144

  describe 'test property getter/setter', ->
    picture = null

    beforeEach ->
      picture = new AVPicture

    afterEach ->
      picture = null

    it 'test default property', ->
      expect(picture.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
      expect(picture.data.length).toBe 8
      expect(picture.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
      expect(picture.width).toBe 0
      expect(picture.height).toBe 0

    it '.pix_fmt', ->
      picture.pix_fmt = AVPixelFormat.AV_PIX_FMT_YUV420P
      expect(picture.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_YUV420P

    it '.width', ->
      picture.width = 720
      expect(picture.width).toBe 720

    it '.height', ->
      picture.height = 480
      expect(picture.height).toBe 480
