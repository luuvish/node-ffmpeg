ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVFormatContext = avformat.AVFormatContext
AVPacket = avcodec.AVPacket
AVFrame = avutil.AVFrame

AVDiscard = avcodec.AVDiscard
AVPixelFormat = avutil.AVPixelFormat
AVSampleFormat = avutil.AVSampleFormat
AVPictureType = avutil.AVPictureType
AVColorRange = avutil.AVColorRange
AVColorPrimaries = avutil.AVColorPrimaries
AVColorTransferCharacteristic = avutil.AVColorTransferCharacteristic
AVColorSpace = avutil.AVColorSpace
AVChromaLocation = avutil.AVChromaLocation


describe 'AVFrame', ->

  describe 'decoding', ->
    [ctx, stream] = [null, null]

    beforeEach ->
      ctx = new AVFormatContext
      ctx.openInput './examples/ticket-a_aac.mkv'

      stream.discard = AVDiscard.AVDISCARD_ALL for stream in ctx.streams

      stream = ctx.streams[0]
      codec = avcodec.findDecoder stream.codec.codec_id
      stream.codec.open codec
      stream.discard = AVDiscard.AVDISCARD_DEFAULT

    afterEach ->
      stream.codec.close()
      ctx.closeInput()
      [ctx, stream] = [null, null]

    it 'decoding 1st frame', ->
      for i in [0...1]
        packet = new AVPacket
        ctx.readFrame packet
        frame = new AVFrame
        [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

      expect(ret).toBe 636
      expect(got_picture_ptr).toBe false

    it 'decoding 2nd frame', ->
      for i in [0...2]
        packet = new AVPacket
        ctx.readFrame packet
        frame = new AVFrame
        [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

      expect(ret).toBe 20
      expect(got_picture_ptr).toBe false

      expect(packet.pts).toBe 160
      expect(packet.size).toBe 20
      expect(packet.pos).toBe 2067745

    it 'decoding 3rd frame', ->
      for i in [0...3]
        packet = new AVPacket
        ctx.readFrame packet
        frame = new AVFrame
        [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

      expect(packet.pts).toBe 80
      expect(packet.size).toBe 18
      expect(packet.pos).toBe 2067777

      expect(ret).toBe 18
      expect(got_picture_ptr).toBe true

      expect(frame.is_writable).toBe false
      expect(frame.data.length).toBe 8
      expect(frame.linesize).toEqual [704, 352, 352, 0, 0, 0, 0, 0]
      expect(frame.width).toBe 688
      expect(frame.height).toBe 288
      expect(frame.nb_samples).toBe 0
      expect(frame.format).toBe AVPixelFormat.AV_PIX_FMT_YUV420P
      expect(frame.key_frame).toBe 1
      expect(frame.pict_type).toBe AVPictureType.AV_PICTURE_TYPE_I
      expect(frame.sample_aspect_ratio).toEqual num: 0, den: 1
      expect(frame.pts).toBe avutil.AV_NOPTS_VALUE
      expect(frame.pkt_pts).toBe 0
      expect(frame.pkt_dts).toBe avutil.AV_NOPTS_VALUE
      expect(frame.coded_picture_number).toBe 0
      expect(frame.display_picture_number).toBe 0
      expect(frame.quality).toBe 0
      expect(frame.repeat_pict).toBe 0
      expect(frame.interlaced_frame).toBe 0
      expect(frame.top_field_first).toBe 0
      expect(frame.palette_has_changed).toBe 0
      expect(frame.reordered_opaque).toBe avutil.AV_NOPTS_VALUE
      expect(frame.sample_rate).toBe 0
      expect(frame.channel_layout).toBe 0
      expect(frame.flags).toBe 0
      expect(frame.color_range).toBe AVColorRange.AVCOL_RANGE_UNSPECIFIED
      expect(frame.color_primaries).toBe AVColorPrimaries.AVCOL_PRI_UNSPECIFIED
      expect(frame.color_trc).toBe AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED
      expect(frame.colorspace).toBe AVColorSpace.AVCOL_SPC_UNSPECIFIED
      expect(frame.chroma_location).toBe AVChromaLocation.AVCHROMA_LOC_LEFT
      expect(frame.best_effort_timestamp).toBe 0
      expect(frame.pkt_pos).toBe 2061676
      expect(frame.pkt_duration).toBe 0
      expect(frame.metadata).toEqual {}
      expect(frame.decode_error_flags).toBe 0
      expect(frame.channels).toBe 0
      expect(frame.pkt_size).toBe 636

    it 'decoding 4th frame', ->
      for i in [0...4]
        packet = new AVPacket
        ctx.readFrame packet
        frame = new AVFrame
        [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

      expect(ret).toBe 18
      expect(got_picture_ptr).toBe true

      expect(packet.pts).toBe 40
      expect(packet.dts).toBe 40
      expect(packet.size).toBe 18
      expect(packet.pos).toBe 2067806

      expect(frame.is_writable).toBe false
      expect(frame.data.length).toBe 8
      expect(frame.linesize).toEqual [704, 352, 352, 0, 0, 0, 0, 0]
      expect(frame.width).toBe 688
      expect(frame.height).toBe 288
      expect(frame.nb_samples).toBe 0
      expect(frame.format).toBe AVPixelFormat.AV_PIX_FMT_YUV420P
      expect(frame.key_frame).toBe 0
      expect(frame.pict_type).toBe AVPictureType.AV_PICTURE_TYPE_B
      expect(frame.sample_aspect_ratio).toEqual num: 0, den: 1
      expect(frame.pts).toBe avutil.AV_NOPTS_VALUE
      expect(frame.pkt_pts).toBe 40
      expect(frame.pkt_dts).toBe 40
      expect(frame.coded_picture_number).toBe 3
      expect(frame.display_picture_number).toBe 0
      expect(frame.quality).toBe 0
      expect(frame.repeat_pict).toBe 0
      expect(frame.interlaced_frame).toBe 0
      expect(frame.top_field_first).toBe 0
      expect(frame.palette_has_changed).toBe 0
      expect(frame.reordered_opaque).toBe avutil.AV_NOPTS_VALUE
      expect(frame.sample_rate).toBe 0
      expect(frame.channel_layout).toBe 0
      expect(frame.flags).toBe 0
      expect(frame.color_range).toBe AVColorRange.AVCOL_RANGE_UNSPECIFIED
      expect(frame.color_primaries).toBe AVColorPrimaries.AVCOL_PRI_UNSPECIFIED
      expect(frame.color_trc).toBe AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED
      expect(frame.colorspace).toBe AVColorSpace.AVCOL_SPC_UNSPECIFIED
      expect(frame.chroma_location).toBe AVChromaLocation.AVCHROMA_LOC_LEFT
      expect(frame.best_effort_timestamp).toBe 40
      expect(frame.pkt_pos).toBe 2067806
      expect(frame.pkt_duration).toBe 40
      expect(frame.metadata).toEqual {}
      expect(frame.decode_error_flags).toBe 0
      expect(frame.channels).toBe 0
      expect(frame.pkt_size).toBe 18

  describe 'test method functions', ->
    [frame0, frame1] = []

    beforeEach ->
      [frame0, frame1] = [new AVFrame, new AVFrame]

    afterEach ->
      [frame0, frame1] = [null, null]

    it '.ref()', ->
      frame0.format = AVPixelFormat.AV_PIX_FMT_YUV420P
      frame0.width = 176
      frame0.height = 144
      frame0.getBuffer 1

      frame1.ref frame0

      expect(frame1.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
      expect(frame1.data[0][28190]).not.toBeUndefined()
      expect(frame1.data[0][28191]).toBeUndefined()
      expect(frame1.data[1][7070]).not.toBeUndefined()
      expect(frame1.data[1][7071]).toBeUndefined()
      expect(frame1.data[2][7070]).not.toBeUndefined()
      expect(frame1.data[2][7071]).toBeUndefined()

    it '.unref()', ->
      frame0.format = AVPixelFormat.AV_PIX_FMT_YUV420P
      frame0.width = 176
      frame0.height = 144
      frame0.getBuffer 1

      frame1.ref frame0
      frame1.unref()

      expect(frame1.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
      expect(frame1.format).toBe -1
      expect(frame1.width).toBe 0
      expect(frame1.height).toBe 0

    it '.moveRef()', ->
      frame0.format = AVPixelFormat.AV_PIX_FMT_YUV420P
      frame0.width = 176
      frame0.height = 144
      frame0.getBuffer 1

      frame1.moveRef frame0

      expect(frame0.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
      expect(frame0.format).toBe -1
      expect(frame0.width).toBe 0
      expect(frame0.height).toBe 0

    it '.getBuffer()', ->
      frame0.format = AVPixelFormat.AV_PIX_FMT_YUV420P
      frame0.width = 176
      frame0.height = 144
      frame0.getBuffer 1

      expect(frame0.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
      expect(frame0.data[0][28190]).not.toBeUndefined()
      expect(frame0.data[0][28191]).toBeUndefined()
      expect(frame0.data[1][7070]).not.toBeUndefined()
      expect(frame0.data[1][7071]).toBeUndefined()
      expect(frame0.data[2][7070]).not.toBeUndefined()
      expect(frame0.data[2][7071]).toBeUndefined()

    it '.copy()', ->
      frame0.format = AVPixelFormat.AV_PIX_FMT_YUV422P
      frame0.width = 640
      frame0.height = 480
      frame0.getBuffer 1

      frame1.format = AVPixelFormat.AV_PIX_FMT_YUV422P
      frame1.width = 640
      frame1.height = 480
      frame1.getBuffer 1

      frame1.copy frame0

    it '.copyProps()', ->
      frame0.sample_aspect_ratio = num: 1, den: 5

      frame1.copyProps frame0

      expect(frame1.sample_aspect_ratio).toEqual num: 1, den: 5

    it '.fillAudioFrame()', ->
      frame0.format = AVSampleFormat.AV_SAMPLE_FMT_S16
      frame0.nb_samples = 8
      frame0.channels = 2
      frame0.getBuffer 1

      buf = new Uint8Array 128
      expect(frame0.fillAudioFrame 2, frame0.format, buf, 1).toBe 32

  describe 'test property getter/setter', ->
    frame = null

    beforeEach ->
      frame = new AVFrame

    afterEach ->
      frame = null

    it 'test default property', ->
      expect(frame.is_writable).toBe false
      expect(frame.data.length).toBe 8
      expect(frame.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
      expect(frame.width).toBe 0
      expect(frame.height).toBe 0
      expect(frame.nb_samples).toBe 0
      expect(frame.format).toBe -1
      expect(frame.key_frame).toBe 1
      expect(frame.pict_type).toBe AVPictureType.AV_PICTURE_TYPE_NONE
      expect(frame.sample_aspect_ratio).toEqual num: 0, den: 1
      expect(frame.pts).toBe avutil.AV_NOPTS_VALUE
      expect(frame.pkt_pts).toBe avutil.AV_NOPTS_VALUE
      expect(frame.pkt_dts).toBe avutil.AV_NOPTS_VALUE
      expect(frame.coded_picture_number).toBe 0
      expect(frame.display_picture_number).toBe 0
      expect(frame.quality).toBe 0
      expect(frame.repeat_pict).toBe 0
      expect(frame.interlaced_frame).toBe 0
      expect(frame.top_field_first).toBe 0
      expect(frame.palette_has_changed).toBe 0
      expect(frame.reordered_opaque).toBe 0
      expect(frame.sample_rate).toBe 0
      expect(frame.channel_layout).toBe 0
      expect(frame.flags).toBe 0
      expect(frame.color_range).toBe AVColorRange.AVCOL_RANGE_UNSPECIFIED
      expect(frame.color_primaries).toBe AVColorPrimaries.AVCOL_PRI_UNSPECIFIED
      expect(frame.color_trc).toBe AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED
      expect(frame.colorspace).toBe AVColorSpace.AVCOL_SPC_UNSPECIFIED
      expect(frame.chroma_location).toBe AVChromaLocation.AVCHROMA_LOC_UNSPECIFIED
      expect(frame.best_effort_timestamp).toBe avutil.AV_NOPTS_VALUE
      expect(frame.pkt_pos).toBe -1
      expect(frame.pkt_duration).toBe 0
      expect(frame.metadata).toEqual {}
      expect(frame.decode_error_flags).toBe 0
      expect(frame.channels).toBe 0
      expect(frame.pkt_size).toBe -1

    it '.is_writable', ->
      frame.is_writable = true
      expect(frame.is_writable).toBe false

    it '.width', ->
      frame.width = 720
      expect(frame.width).toBe 720

    it '.height', ->
      frame.height = 480
      expect(frame.height).toBe 480

    it '.nb_samples', ->
      frame.nb_samples = 44000
      expect(frame.nb_samples).toBe 44000

    it '.format', ->
      frame.format = AVPixelFormat.AV_PIX_FMT_YUV420P
      expect(frame.format).toBe AVPixelFormat.AV_PIX_FMT_YUV420P

    it '.key_frame', ->
      expect(frame.key_frame).toBe 1

    it '.pict_type', ->
      frame.pict_type = AVPictureType.AV_PICTURE_TYPE_I
      expect(frame.pict_type).toBe AVPictureType.AV_PICTURE_TYPE_I

    it '.sample_aspect_ratio', ->
      frame.sample_aspect_ratio = num: 10, den: 13
      expect(frame.sample_aspect_ratio).toEqual num: 10, den: 13

    it '.pts', ->
      frame.pts = 100
      expect(frame.pts).toBe 100

    it '.pkt_pts', ->
      expect(frame.pkt_pts).toBe avutil.AV_NOPTS_VALUE

    it '.pkt_dts', ->
      expect(frame.pkt_dts).toBe avutil.AV_NOPTS_VALUE

    it '.coded_picture_number', ->
      expect(frame.coded_picture_number).toBe 0

    it '.display_picture_number', ->
      expect(frame.display_picture_number).toBe 0

    it '.quality', ->
      frame.quality = 4
      expect(frame.quality).toBe 4

    it '.repeat_pict', ->
      frame.repeat_pict = 5
      expect(frame.repeat_pict).toBe 5

    it '.interlaced_frame', ->
      frame.interlaced_frame = 1
      expect(frame.interlaced_frame).toBe 1

    it '.top_field_first', ->
      frame.top_field_first = 1
      expect(frame.top_field_first).toBe 1

    it '.palette_has_changed', ->
      expect(frame.palette_has_changed).toBe 0

    it '.reordered_opaque', ->
      expect(frame.reordered_opaque).toBe 0

    it '.sample_rate', ->
      frame.sample_rate = 44000
      expect(frame.sample_rate).toBe 44000

    it '.channel_layout', ->
      frame.channel_layout = 2
      expect(frame.channel_layout).toBe 2

    it '.flags', ->
      frame.flags = 1234
      expect(frame.flags).toBe 1234

    it '.color_range', ->
      frame.color_range = AVColorRange.AVCOL_RANGE_JPEG
      expect(frame.color_range).toBe AVColorRange.AVCOL_RANGE_JPEG

    it '.color_primaries', ->
      expect(frame.color_primaries).toBe AVColorPrimaries.AVCOL_PRI_UNSPECIFIED

    it '.color_trc', ->
      expect(frame.color_trc).toBe AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED

    it '.colorspace', ->
      frame.colorspace = AVColorSpace.AVCOL_SPC_RGB
      expect(frame.colorspace).toBe AVColorSpace.AVCOL_SPC_RGB

    it '.chroma_location', ->
      expect(frame.chroma_location).toBe AVChromaLocation.AVCHROMA_LOC_UNSPECIFIED

    it '.best_effort_timestamp', ->
      frame.best_effort_timestamp = 55
      expect(frame.best_effort_timestamp).toBe 55

    it '.pkt_pos', ->
      frame.pkt_pos = 77
      expect(frame.pkt_pos).toBe 77

    it '.pkt_duration', ->
      frame.pkt_duration = 33
      expect(frame.pkt_duration).toBe 33

    it '.metadata', ->
      frame.metadata = a: 'a', b: 'c'
      expect(frame.metadata).toEqual a: 'a', b: 'c'

    it '.decode_error_flags', ->
      frame.decode_error_flags = 11
      expect(frame.decode_error_flags).toBe 11

    it '.channels', ->
      frame.channels = 5
      expect(frame.channels).toBe 5

    it '.pkt_size', ->
      frame.pkt_size = 30
      expect(frame.pkt_size).toBe 30
