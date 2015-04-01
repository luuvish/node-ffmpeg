ffmpeg = require '../../lib/ffmpeg'

describe 'AVProgram', ->
  avformat = ffmpeg.avformat
  avutil = ffmpeg.avutil
  AVDiscard = ffmpeg.avcodec.AVDiscard

  it 'output AVProgram', ->
    ctx = new avformat.AVFormatContext
    ctx.openOutput 'mp4', 'output.mp4'

    program = ctx.newProgram 3

    expect(ctx.programs.length).toBe 1

    expect(program.id).toBe 3
    expect(program.flags).toBe 0
    expect(program.discard).toBe AVDiscard.AVDISCARD_NONE
    expect(program.stream_indexes).toEqual []
    expect(program.metadata).toEqual {}
    expect(program.program_num).toBe 0
    expect(program.pmt_pid).toBe 0
    expect(program.pcr_pid).toBe 0
    expect(program.start_time).toBe avutil.AV_NOPTS_VALUE
    expect(program.end_time).toBe avutil.AV_NOPTS_VALUE
    expect(program.pts_wrap_reference).toBe avutil.AV_NOPTS_VALUE
    expect(program.pts_wrap_behavior).toBe 0

    ctx.closeOutput()
    ctx = null

  it 'AVProgram getter/setter test', ->
    ctx = new avformat.AVFormatContext
    ctx.openOutput 'mp4', 'output.mp4'

    program = ctx.newProgram 5

    program.flags = 9
    expect(program.flags).toBe 9

    program.discard = AVDiscard.AVDISCARD_ALL
    expect(program.discard).toBe AVDiscard.AVDISCARD_ALL

    program.program_num = 7
    expect(program.program_num).toBe 7

    program.pmt_pid = 100
    expect(program.pmt_pid).toBe 100

    program.pcr_pid = 200
    expect(program.pcr_pid).toBe 200

    ctx.closeOutput()
    ctx = null
