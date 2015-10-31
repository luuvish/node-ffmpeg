ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVFormatContext = avformat.AVFormatContext
AVPacket = avcodec.AVPacket


describe 'AVPacket', ->

  describe 'decoding', ->
    ctx = null

    beforeEach ->
      ctx = new AVFormatContext
      ctx.openInput './examples/ticket-a_aac.mkv'

    afterEach ->
      ctx.closeInput()
      ctx = null

    it 'decoding 1st packet', ->
      for i in [0...1]
        packet = new AVPacket
        ctx.readFrame packet

      expect(packet.pts).toBe 0
      expect(packet.dts).toBe avutil.AV_NOPTS_VALUE
      expect(packet.data[0]).toBe 0
      expect(packet.data[1]).toBe 0
      expect(packet.data[2]).toBe 2
      expect(packet.data[3]).toBe 35
      expect(packet.data[4]).toBe 6
      expect(packet.data[634]).toBe 16
      expect(packet.data[635]).toBe 16
      expect(packet.size).toBe 636
      expect(packet.stream_index).toBe 0
      expect(packet.flags).toBe 1
      expect(packet.duration).toBe 0
      expect(packet.pos).toBe 2061676
      expect(packet.convergence_duration).toBe 0

    it 'decoding 2nd packet', ->
      for i in [0...2]
        packet = new AVPacket
        ctx.readFrame packet

      expect(packet.pts).toBe 0
      expect(packet.dts).toBe 0
      expect(packet.data[0]).toBe 1
      expect(packet.data[1]).toBe 64
      expect(packet.data[2]).toBe 32
      expect(packet.data[3]).toBe 6
      expect(packet.data[4]).toBe 189
      expect(packet.data[675]).toBe 0
      expect(packet.data[676]).toBe 63
      expect(packet.size).toBe 677
      expect(packet.stream_index).toBe 1
      expect(packet.flags).toBe 1
      expect(packet.duration).toBe 42
      expect(packet.pos).toBe 2062322
      expect(packet.convergence_duration).toBe 0

    it 'decoding 3rd packet', ->
      for i in [0...3]
        packet = new AVPacket
        ctx.readFrame packet

      expect(packet.pts).toBe 42
      expect(packet.dts).toBe 42
      expect(packet.data[0]).toBe 1
      expect(packet.data[1]).toBe 64
      expect(packet.data[2]).toBe 32
      expect(packet.data[3]).toBe 6
      expect(packet.data[4]).toBe 141
      expect(packet.data[673]).toBe 0
      expect(packet.data[674]).toBe 63
      expect(packet.size).toBe 675
      expect(packet.stream_index).toBe 1
      expect(packet.flags).toBe 1
      expect(packet.duration).toBe 43
      expect(packet.pos).toBe 2062322
      expect(packet.convergence_duration).toBe 0

  describe 'test method functions', ->
    [packet0, packet1] = [null, null]

    beforeEach ->
      [packet0, packet1] = [new AVPacket, new AVPacket]

    afterEach ->
      [packet0, packet1] = [null, null]

    it '.dup()', ->
      packet0.dup()

    it '.copy()', ->
      packet1.copy packet0

    it '.copySideData()', ->
      packet0.copySideData packet1

    it '.newSideData()', ->
      packet0.newSideData 1, 100

    it '.getSideData()', ->
      packet0.newSideData 1, 100
      expect(packet0.getSideData 1).toEqual '0': 0

    it '.mergeSideData()', ->
      packet0.newSideData 1, 100
      expect(packet0.mergeSideData()).toBe 1

    it '.splitSideData()', ->
      packet0.newSideData 1, 100
      packet0.mergeSideData()
      expect(packet0.splitSideData()).toBe 1

    it '.rescaleTs()', ->
      packet0.rescaleTs {num: 1, den: 5}, {num: 10, den: 3}

  describe 'test property getter/setter', ->
    packet = null

    beforeEach ->
      packet = new AVPacket

    afterEach ->
      packet = null

    it 'test default property', ->
      expect(packet.pts).toBe avutil.AV_NOPTS_VALUE
      expect(packet.dts).toBe avutil.AV_NOPTS_VALUE
      expect(packet.size).toBe 0
      expect(packet.stream_index).toBe 0
      expect(packet.flags).toBe 0
      expect(packet.duration).toBe 0
      expect(packet.pos).toBe -1
      expect(packet.convergence_duration).toBe 0

    it '.pts', ->
      packet.pts = 777
      expect(packet.pts).toBe 777

    it '.dts', ->
      packet.dts = 49
      expect(packet.dts).toBe 49

    it '.stream_index', ->
      packet.stream_index = 3
      expect(packet.stream_index).toBe 3

    it '.flags', ->
      packet.flags = 41
      expect(packet.flags).toBe 41

    it '.duration', ->
      packet.duration = 10000
      expect(packet.duration).toBe 10000

    it '.pos', ->
      packet.pos = 200
      expect(packet.pos).toBe 200

    it '.convergence_duration', ->
      packet.convergence_duration = 50
      expect(packet.convergence_duration).toBe 50
