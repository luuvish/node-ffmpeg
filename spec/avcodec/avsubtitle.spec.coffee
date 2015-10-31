ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVFormatContext = avformat.AVFormatContext
AVPacket = avcodec.AVPacket
AVSubtitle = avcodec.AVSubtitle

AVDiscard = avcodec.AVDiscard
AVSubtitleType = avcodec.AVSubtitleType
AVPixelFormat = avutil.AVPixelFormat


describe 'AVPacket', ->

  describe 'decoding', ->
    [ctx, stream] = [null, null]

    beforeEach ->
      ctx = new AVFormatContext
      ctx.openInput './examples/h264-TTA.mkv'

      stream.discard = AVDiscard.AVDISCARD_ALL for stream in ctx.streams

      stream = ctx.streams[2]
      codec = avcodec.findDecoder stream.codec.codec_id
      stream.codec.open codec
      stream.discard = AVDiscard.AVDISCARD_DEFAULT

    afterEach ->
      stream.codec.close()
      ctx.closeInput()
      [ctx, stream] = [null, null]

    it 'decoding 1st subtitle', ->
      for i in [0...1]
        packet = new AVPacket
        ctx.readFrame packet
        subtitle = new AVSubtitle
        [ret, got_sub_ptr] = stream.codec.decodeSubtitle subtitle, packet

      expect(ret).toBe 98
      expect(got_sub_ptr).toBe true

      expect(packet.pts).toBe 2070
      expect(packet.dts).toBe 2070
      expect(packet.data[0]).toBe 55
      expect(packet.data[1]).toBe 51
      expect(packet.data[96]).toBe 152
      expect(packet.data[97]).toBe 134
      expect(packet.size).toBe 98
      expect(packet.stream_index).toBe 2
      expect(packet.flags).toBe 1
      expect(packet.duration).toBe 1200
      expect(packet.pos).toBe 469421
      expect(packet.convergence_duration).toBe 0

      expect(subtitle.format).toBe 1
      expect(subtitle.start_display_time).toBe 0
      expect(subtitle.end_display_time).toBe 1200
      expect(subtitle.rects[0].x).toBe 0
      expect(subtitle.rects[0].y).toBe 0
      expect(subtitle.rects[0].w).toBe 0
      expect(subtitle.rects[0].h).toBe 0
      expect(subtitle.rects[0].nb_colors).toBe 0
      expect(subtitle.rects[0].pict.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
      expect(subtitle.rects[0].pict.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
      expect(subtitle.rects[0].pict.width).toBe 0
      expect(subtitle.rects[0].pict.height).toBe 0
      expect(subtitle.rects[0].type).toBe AVSubtitleType.SUBTITLE_ASS
      expect(subtitle.rects[0].text).toBe ''
      expect(subtitle.rects[0].ass).toMatch /^Dialogue: /
      expect(subtitle.rects[0].flags).toBe 0
      expect(subtitle.pts).toBe 2070000

    it 'decoding 1st subtitle', ->
      for i in [0...2]
        packet = new AVPacket
        ctx.readFrame packet
        subtitle = new AVSubtitle
        [ret, got_sub_ptr] = stream.codec.decodeSubtitle subtitle, packet

      expect(ret).toBe 135
      expect(got_sub_ptr).toBe true

      expect(packet.pts).toBe 2070
      expect(packet.dts).toBe 2070
      expect(packet.data[0]).toBe 55
      expect(packet.data[1]).toBe 56
      expect(packet.data[133]).toBe 114
      expect(packet.data[134]).toBe 125
      expect(packet.size).toBe 135
      expect(packet.stream_index).toBe 2
      expect(packet.flags).toBe 1
      expect(packet.duration).toBe 1200
      expect(packet.pos).toBe 469533
      expect(packet.convergence_duration).toBe 0

      expect(subtitle.format).toBe 1
      expect(subtitle.start_display_time).toBe 0
      expect(subtitle.end_display_time).toBe 1200
      expect(subtitle.rects[0].x).toBe 0
      expect(subtitle.rects[0].y).toBe 0
      expect(subtitle.rects[0].w).toBe 0
      expect(subtitle.rects[0].h).toBe 0
      expect(subtitle.rects[0].nb_colors).toBe 0
      expect(subtitle.rects[0].pict.linesize).toEqual [0, 0, 0, 0, 0, 0, 0, 0]
      expect(subtitle.rects[0].pict.pix_fmt).toBe AVPixelFormat.AV_PIX_FMT_NONE
      expect(subtitle.rects[0].pict.width).toBe 0
      expect(subtitle.rects[0].pict.height).toBe 0
      expect(subtitle.rects[0].type).toBe AVSubtitleType.SUBTITLE_ASS
      expect(subtitle.rects[0].text).toBe ''
      expect(subtitle.rects[0].ass).toMatch /^Dialogue: /
      expect(subtitle.rects[0].flags).toBe 0
      expect(subtitle.pts).toBe 2070000

  describe 'test property getter/setter', ->
    subtitle = null

    beforeEach ->
      subtitle = new AVSubtitle

    afterEach ->
      subtitle.free()
      subtitle = null

    it 'test default property', ->
      expect(subtitle.format).toBe AVSubtitleType.SUBTITLE_NONE
      expect(subtitle.start_display_time).toBe 0
      expect(subtitle.end_display_time).toBe 0
      expect(subtitle.rects).toEqual []
      expect(subtitle.pts).toBe 0

    it '.format', ->
      subtitle.format = AVSubtitleType.SUBTITLE_ASS
      expect(subtitle.format).toBe AVSubtitleType.SUBTITLE_ASS

    it '.start_display_time', ->
      subtitle.start_display_time = 20
      expect(subtitle.start_display_time).toBe 20

    it '.end_display_time', ->
      subtitle.end_display_time = 40
      expect(subtitle.end_display_time).toBe 40

    it '.pts', ->
      subtitle.pts = 200
      expect(subtitle.pts).toBe 200
