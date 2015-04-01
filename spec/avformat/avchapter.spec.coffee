ffmpeg = require '../../lib/ffmpeg'

describe 'AVChapter', ->
  avformat = ffmpeg.avformat

  it 'input AVChapter', ->
    ctx = new avformat.AVFormatContext
    fmt = avformat.findInputFormat 'matroska'
    ctx.openInput './examples/Mushishi24-head.mkv'

    expect(ctx.chapters.length).toBe 5

    expect(ctx.chapters[0].id).toBe -1230360014
    expect(ctx.chapters[0].time_base).toEqual {num: 1, den: 1000000000}
    expect(ctx.chapters[0].start).toBe 0
    expect(ctx.chapters[0].end).toBe 99992000000
    expect(ctx.chapters[0].metadata).toEqual {title: 'OP'}

    expect(ctx.chapters[1].id).toBe 184427016
    expect(ctx.chapters[1].time_base).toEqual {num: 1, den: 1000000000}
    expect(ctx.chapters[1].start).toBe 99992000000
    expect(ctx.chapters[1].end).toBe 669972000000
    expect(ctx.chapters[1].metadata).toEqual {title: 'A part'}

    expect(ctx.chapters[2].id).toBe -2020208942
    expect(ctx.chapters[2].time_base).toEqual {num: 1, den: 1000000000}
    expect(ctx.chapters[2].start).toBe 669972000000
    expect(ctx.chapters[2].end).toBe 1330018000000
    expect(ctx.chapters[2].metadata).toEqual {title: 'B part'}

    expect(ctx.chapters[3].id).toBe -1526018202
    expect(ctx.chapters[3].time_base).toEqual {num: 1, den: 1000000000}
    expect(ctx.chapters[3].start).toBe 1330018000000
    expect(ctx.chapters[3].end).toBe 1420027000000
    expect(ctx.chapters[3].metadata).toEqual {title: 'ED'}

    expect(ctx.chapters[4].id).toBe -1676258626
    expect(ctx.chapters[4].time_base).toEqual {num: 1, den: 1000000000}
    expect(ctx.chapters[4].start).toBe 1420027000000
    expect(ctx.chapters[4].end).toBe 1462449000000
    expect(ctx.chapters[4].metadata).toEqual {title: 'next'}

    ctx.closeInput()
    ctx = null
