ffmpeg = require '../../lib/ffmpeg'

describe 'AVFrame', ->
  avformat = ffmpeg.avformat
  avcodec = ffmpeg.avcodec
  avutil = ffmpeg.avutil
  AVDiscard = ffmpeg.avcodec.AVDiscard
  AVPictureType = ffmpeg.avutil.AVPictureType

  ctx = new avformat.AVFormatContext
  ctx.openInput './examples/ticket-a_aac.mkv'

  stream.discard = AVDiscard.AVDISCARD_ALL for stream in ctx.streams

  stream = ctx.streams[0]
  codec = avcodec.findDecoder stream.codec.codec_id
  stream.codec.open codec
  stream.discard = AVDiscard.AVDISCARD_DEFAULT

  it 'create video AVFrame', ->
    frame = new avutil.AVFrame

    expect(frame.is_writable).toBe false
    expect(frame.data.length).toBe 8
    expect(frame.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
    expect(frame.width).toBe 0
    expect(frame.height).toBe 0
    expect(frame.nb_samples).toBe 0
    expect(frame.format).toBe -1
    expect(frame.key_frame).toBe 1
    expect(frame.pict_type).toBe AVPictureType.AV_PICTURE_TYPE_NONE
    expect(frame.sample_aspect_ratio).toEqual {num: 0, den: 1}
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
    expect(frame.color_range).toBe avutil.AVColorRange.AVCOL_RANGE_UNSPECIFIED
    expect(frame.color_primaries).toBe avutil.AVColorPrimaries.AVCOL_PRI_UNSPECIFIED
    expect(frame.color_trc).toBe avutil.AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED
    expect(frame.chroma_location).toBe avutil.AVChromaLocation.AVCHROMA_LOC_UNSPECIFIED
    expect(frame.best_effort_timestamp).toBe avutil.AV_NOPTS_VALUE
    expect(frame.pkt_pos).toBe -1
    expect(frame.pkt_duration).toBe 0
    expect(frame.metadata).toEqual {}
    expect(frame.decode_error_flags).toBe 0
    expect(frame.channels).toBe 0
    expect(frame.pkt_size).toBe -1

  it 'decode AVFrame', ->
    packet = new avcodec.AVPacket
    ctx.readFrame packet
    frame = new avutil.AVFrame
    [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

    expect(ret).toBe 636
    expect(got_picture_ptr).toBe false

    packet = new avcodec.AVPacket
    ctx.readFrame packet

    expect(packet.pts).toBe 160
    expect(packet.size).toBe 20
    expect(packet.pos).toBe 2067745

    frame = new avutil.AVFrame
    [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

    expect(ret).toBe 20
    expect(got_picture_ptr).toBe false

    packet = new avcodec.AVPacket
    ctx.readFrame packet

    expect(packet.pts).toBe 80
    expect(packet.size).toBe 18
    expect(packet.pos).toBe 2067777

    frame = new avutil.AVFrame
    [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

    expect(ret).toBe 18
    expect(got_picture_ptr).toBe true

    expect(frame.is_writable).toBe false
    expect(frame.data.length).toBe 8
    expect(frame.linesize).toEqual [704, 352, 352, 0, 0, 0, 0, 0]
    expect(frame.width).toBe 688
    expect(frame.height).toBe 288
    expect(frame.nb_samples).toBe 0
    expect(frame.format).toBe avutil.AVPixelFormat.AV_PIX_FMT_YUV420P
    expect(frame.key_frame).toBe 1
    expect(frame.pict_type).toBe AVPictureType.AV_PICTURE_TYPE_I
    expect(frame.sample_aspect_ratio).toEqual {num: 0, den: 1}
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
    expect(frame.color_range).toBe avutil.AVColorRange.AVCOL_RANGE_UNSPECIFIED
    expect(frame.color_primaries).toBe avutil.AVColorPrimaries.AVCOL_PRI_UNSPECIFIED
    expect(frame.color_trc).toBe avutil.AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED
    expect(frame.chroma_location).toBe avutil.AVChromaLocation.AVCHROMA_LOC_LEFT
    expect(frame.best_effort_timestamp).toBe 0
    expect(frame.pkt_pos).toBe 2061676
    expect(frame.pkt_duration).toBe 0
    expect(frame.metadata).toEqual {}
    expect(frame.decode_error_flags).toBe 0
    expect(frame.channels).toBe 0
    expect(frame.pkt_size).toBe 636

    packet = new avcodec.AVPacket
    ctx.readFrame packet

    expect(packet.pts).toBe 40
    expect(packet.dts).toBe 40
    expect(packet.size).toBe 18
    expect(packet.pos).toBe 2067806

    frame = new avutil.AVFrame
    [ret, got_picture_ptr] = stream.codec.decodeVideo frame, packet

    expect(ret).toBe 18
    expect(got_picture_ptr).toBe true

    expect(frame.is_writable).toBe false
    expect(frame.data.length).toBe 8
    expect(frame.linesize).toEqual [704, 352, 352, 0, 0, 0, 0, 0]
    expect(frame.width).toBe 688
    expect(frame.height).toBe 288
    expect(frame.nb_samples).toBe 0
    expect(frame.format).toBe avutil.AVPixelFormat.AV_PIX_FMT_YUV420P
    expect(frame.key_frame).toBe 0
    expect(frame.pict_type).toBe AVPictureType.AV_PICTURE_TYPE_B
    expect(frame.sample_aspect_ratio).toEqual {num: 0, den: 1}
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
    expect(frame.color_range).toBe avutil.AVColorRange.AVCOL_RANGE_UNSPECIFIED
    expect(frame.color_primaries).toBe avutil.AVColorPrimaries.AVCOL_PRI_UNSPECIFIED
    expect(frame.color_trc).toBe avutil.AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED
    expect(frame.chroma_location).toBe avutil.AVChromaLocation.AVCHROMA_LOC_LEFT
    expect(frame.best_effort_timestamp).toBe 40
    expect(frame.pkt_pos).toBe 2067806
    expect(frame.pkt_duration).toBe 40
    expect(frame.metadata).toEqual {}
    expect(frame.decode_error_flags).toBe 0
    expect(frame.channels).toBe 0
    expect(frame.pkt_size).toBe 18

  it 'AVFrame.ref() test', ->
    frame0 = new avutil.AVFrame
    frame0.format = avutil.AVPixelFormat.AV_PIX_FMT_YUV420P
    frame0.width = 176
    frame0.height = 144
    frame0.getBuffer 1

    frame1 = new avutil.AVFrame
    frame1.ref frame0

    expect(frame1.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
    expect(frame1.data[0][28190]).not.toBe undefined
    expect(frame1.data[0][28191]).toBe undefined
    expect(frame1.data[1][7070]).not.toBe undefined
    expect(frame1.data[1][7071]).toBe undefined
    expect(frame1.data[2][7070]).not.toBe undefined
    expect(frame1.data[2][7071]).toBe undefined

  it 'AVFrame.unref() test', ->
    frame0 = new avutil.AVFrame
    frame0.format = avutil.AVPixelFormat.AV_PIX_FMT_YUV420P
    frame0.width = 176
    frame0.height = 144
    frame0.getBuffer 1

    frame1 = new avutil.AVFrame
    frame1.ref frame0
    frame1.unref()

    expect(frame1.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
    expect(frame1.format).toBe -1
    expect(frame1.width).toBe 0
    expect(frame1.height).toBe 0

  it 'AVFrame.moveRef() test', ->
    frame0 = new avutil.AVFrame
    frame0.format = avutil.AVPixelFormat.AV_PIX_FMT_YUV420P
    frame0.width = 176
    frame0.height = 144
    frame0.getBuffer 1

    frame1 = new avutil.AVFrame
    frame1.moveRef frame0

    expect(frame0.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
    expect(frame0.format).toBe -1
    expect(frame0.width).toBe 0
    expect(frame0.height).toBe 0

  it 'AVFrame.getBuffer() test', ->
    frame = new avutil.AVFrame
    frame.format = avutil.AVPixelFormat.AV_PIX_FMT_YUV420P
    frame.width = 176
    frame.height = 144
    frame.getBuffer 1

    expect(frame.linesize).toEqual [176, 88, 88, 0, 0, 0, 0, 0]
    expect(frame.data[0][28190]).not.toBe undefined
    expect(frame.data[0][28191]).toBe undefined
    expect(frame.data[1][7070]).not.toBe undefined
    expect(frame.data[1][7071]).toBe undefined
    expect(frame.data[2][7070]).not.toBe undefined
    expect(frame.data[2][7071]).toBe undefined

  it 'AVFrame.copy() test', ->
    frame0 = new avutil.AVFrame
    frame0.format = avutil.AVPixelFormat.AV_PIX_FMT_YUV422P
    frame0.width = 640
    frame0.height = 480
    frame0.getBuffer 1

    frame1 = new avutil.AVFrame
    frame1.format = avutil.AVPixelFormat.AV_PIX_FMT_YUV422P
    frame1.width = 640
    frame1.height = 480
    frame1.getBuffer 1

    frame1.copy frame0

  it 'AVFrame.copyProps() test', ->
    frame0 = new avutil.AVFrame
    frame0.sample_aspect_ratio = num: 1, den: 5

    frame1 = new avutil.AVFrame
    frame1.copyProps frame0

    expect(frame1.sample_aspect_ratio).toEqual num: 1, den: 5

  it 'AVFrame getter/setter test', ->
    frame = new avutil.AVFrame

    frame.is_writable = true
    expect(frame.is_writable).toBe false

    frame.width = 720
    expect(frame.width).toBe 720

    frame.height = 480
    expect(frame.height).toBe 480

    frame.nb_samples = 44000
    expect(frame.nb_samples).toBe 44000

    frame.format = avutil.AVPixelFormat.AV_PIX_FMT_YUV420P
    expect(frame.format).toBe avutil.AVPixelFormat.AV_PIX_FMT_YUV420P

    expect(frame.key_frame).toBe 1

    frame.pict_type = avutil.AVPictureType.AV_PICTURE_TYPE_I
    expect(frame.pict_type).toBe avutil.AVPictureType.AV_PICTURE_TYPE_I

    frame.sample_aspect_ratio = num: 10, den: 13
    expect(frame.sample_aspect_ratio).toEqual num: 10, den: 13

    frame.pts = 100
    expect(frame.pts).toBe 100

    expect(frame.pkt_pts).toBe avutil.AV_NOPTS_VALUE
    expect(frame.pkt_dts).toBe avutil.AV_NOPTS_VALUE
    expect(frame.coded_picture_number).toBe 0
    expect(frame.display_picture_number).toBe 0

    frame.quality = 4
    expect(frame.quality).toBe 4

    frame.repeat_pict = 5
    expect(frame.repeat_pict).toBe 5

    frame.interlaced_frame = 1
    expect(frame.interlaced_frame).toBe 1

    frame.top_field_first = 1
    expect(frame.top_field_first).toBe 1

    expect(frame.palette_has_changed).toBe 0
    expect(frame.reordered_opaque).toBe 0

    frame.sample_rate = 44000
    expect(frame.sample_rate).toBe 44000

    frame.channel_layout = 2
    expect(frame.channel_layout).toBe 2

    frame.flags = 1234
    expect(frame.flags).toBe 1234

    frame.color_range = avutil.AVColorRange.AVCOL_RANGE_JPEG
    expect(frame.color_range).toBe avutil.AVColorRange.AVCOL_RANGE_JPEG

    expect(frame.color_primaries).toBe avutil.AVColorPrimaries.AVCOL_PRI_UNSPECIFIED
    expect(frame.color_trc).toBe avutil.AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED

    frame.colorspace = avutil.AVColorSpace.AVCOL_SPC_RGB
    expect(frame.colorspace).toBe avutil.AVColorSpace.AVCOL_SPC_RGB

    expect(frame.chroma_location).toBe avutil.AVChromaLocation.AVCHROMA_LOC_UNSPECIFIED

    frame.best_effort_timestamp = 55
    expect(frame.best_effort_timestamp).toBe 55

    frame.pkt_pos = 77
    expect(frame.pkt_pos).toBe 77

    frame.pkt_duration = 33
    expect(frame.pkt_duration).toBe 33

    frame.metadata = a: 'a', b: 'c'
    expect(frame.metadata).toEqual a: 'a', b: 'c'

    frame.decode_error_flags = 11
    expect(frame.decode_error_flags).toBe 11

    frame.channels = 5
    expect(frame.channels).toBe 5

    frame.pkt_size = 30
    expect(frame.pkt_size).toBe 30
