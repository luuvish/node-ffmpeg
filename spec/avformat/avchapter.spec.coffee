ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat

AVFormatContext = avformat.AVFormatContext


describe 'AVChapter', ->

  describe 'open AVChapter', ->
    ctx = null

    beforeEach ->
      ctx = new AVFormatContext
      fmt = avformat.findInputFormat 'matroska'
      ctx.openInput './examples/Mushishi24-head.mkv'

    afterEach ->
      ctx.closeInput()
      ctx = null

    it 'open 5 chapters', ->
      expect(ctx.chapters.length).toBe 5

    it 'chapter[0]', ->
      chapter = ctx.chapters[0]

      expect(chapter.id).toBe -1230360014
      expect(chapter.time_base).toEqual num: 1, den: 1000000000
      expect(chapter.start).toBe 0
      expect(chapter.end).toBe 99992000000
      expect(chapter.metadata).toEqual title: 'OP'

    it 'chapter[1]', ->
      chapter = ctx.chapters[1]

      expect(chapter.id).toBe 184427016
      expect(chapter.time_base).toEqual num: 1, den: 1000000000
      expect(chapter.start).toBe 99992000000
      expect(chapter.end).toBe 669972000000
      expect(chapter.metadata).toEqual title: 'A part'

    it 'chapter[2]', ->
      chapter = ctx.chapters[2]

      expect(chapter.id).toBe -2020208942
      expect(chapter.time_base).toEqual num: 1, den: 1000000000
      expect(chapter.start).toBe 669972000000
      expect(chapter.end).toBe 1330018000000
      expect(chapter.metadata).toEqual title: 'B part'

    it 'chapter[3]', ->
      chapter = ctx.chapters[3]

      expect(chapter.id).toBe -1526018202
      expect(chapter.time_base).toEqual num: 1, den: 1000000000
      expect(chapter.start).toBe 1330018000000
      expect(chapter.end).toBe 1420027000000
      expect(chapter.metadata).toEqual title: 'ED'

    it 'chapter[4]', ->
      chapter = ctx.chapters[4]

      expect(chapter.id).toBe -1676258626
      expect(chapter.time_base).toEqual num: 1, den: 1000000000
      expect(chapter.start).toBe 1420027000000
      expect(chapter.end).toBe 1462449000000
      expect(chapter.metadata).toEqual title: 'next'
