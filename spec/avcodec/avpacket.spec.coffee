ffmpeg = require '../../lib/ffmpeg'

describe 'AVPacket', ->
  avformat = ffmpeg.avformat
  avcodec = ffmpeg.avcodec
  avutil = ffmpeg.avutil

  ctx = new avformat.AVFormatContext
  ctx.openInput './examples/ticket-a_aac.mkv'

  it 'create AVPacket', ->
    packet = new avcodec.AVPacket

    expect(packet.pts).toBe avutil.AV_NOPTS_VALUE
    expect(packet.dts).toBe avutil.AV_NOPTS_VALUE
    expect(packet.size).toBe 0
    expect(packet.stream_index).toBe 0
    expect(packet.flags).toBe 0
    expect(packet.duration).toBe 0
    expect(packet.pos).toBe -1
    expect(packet.convergence_duration).toBe 0

  it 'read AVPacket', ->
    packet = new avcodec.AVPacket
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

    packet = new avcodec.AVPacket
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

    packet = new avcodec.AVPacket
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

  it 'AVPacket.dup() test', ->
    packet = new avcodec.AVPacket
    packet.dup()

  it 'AVPacket.copy() test', ->
    packet = new avcodec.AVPacket
    pkt2 = new avcodec.AVPacket
    pkt2.copy packet

  xit 'AVPacket.CopySideData() test', ->
  xit 'AVPacket.NewSideData() test', ->
  xit 'AVPacket.GetSideData() test', ->
  xit 'AVPacket.MergeSideData() test', ->
  xit 'AVPacket.SplitSideData() test', ->

  it 'AVPacket.RescaleTs() test', ->
    packet = new avcodec.AVPacket
    packet.rescaleTs {num: 1, den: 5}, {num: 10, den: 3}

  it 'AVPacket getter/setter test', ->
    packet = new avcodec.AVPacket

    packet.pts = 777
    expect(packet.pts).toBe 777

    packet.dts = 49
    expect(packet.dts).toBe 49

    packet.stream_index = 3
    expect(packet.stream_index).toBe 3

    packet.flags = 41
    expect(packet.flags).toBe 41

    packet.duration = 10000
    expect(packet.duration).toBe 10000

    packet.pos = 200
    expect(packet.pos).toBe 200

    packet.convergence_duration = 50
    expect(packet.convergence_duration).toBe 50
