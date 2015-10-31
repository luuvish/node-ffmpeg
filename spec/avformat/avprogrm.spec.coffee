ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVFormatContext = avformat.AVFormatContext

AVDiscard = avcodec.AVDiscard


describe 'AVProgram', ->

  describe 'create AVProgram', ->
    [ctx, program] = [null, null]

    beforeEach ->
      ctx = new AVFormatContext
      ctx.openOutput 'mp4', 'output.mp4'
      program = ctx.newProgram 3

    afterEach ->
      ctx.closeOutput()
      [ctx, program] = [null, null]

    it 'one program created', ->
      expect(ctx.programs.length).toBe 1

  describe 'test property getter/setter', ->
    [ctx, program] = [null, null]

    beforeEach ->
      ctx = new AVFormatContext
      ctx.openOutput 'mp4', 'output.mp4'
      program = ctx.newProgram 5

    afterEach ->
      ctx.closeOutput()
      [ctx, program] = [null, null]

    it 'test default property', ->
      expect(program.id).toBe 5
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

    it '.flags', ->
      program.flags = 9
      expect(program.flags).toBe 9

    it '.discard', ->
      program.discard = AVDiscard.AVDISCARD_ALL
      expect(program.discard).toBe AVDiscard.AVDISCARD_ALL

    it '.program_num', ->
      program.program_num = 7
      expect(program.program_num).toBe 7

    it '.pmt_pid', ->
      program.pmt_pid = 100
      expect(program.pmt_pid).toBe 100

    it '.pcr_pid', ->
      program.pcr_pid = 200
      expect(program.pcr_pid).toBe 200
