ffmpeg = require '../../lib/ffmpeg'

describe 'ffmpeg.avformat', ->
  avformat = ffmpeg.avformat

  it 'avformat functions', ->
    expect(avformat.version()).toBe 3676517
    expect(avformat.license()).toBe 'LGPL version 2.1 or later'

  it 'avformat probe score constants', ->
    expect(avformat.AVPROBE_SCORE_RETRY).toBe 25
    expect(avformat.AVPROBE_SCORE_STREAM_RETRY).toBe 24
    expect(avformat.AVPROBE_SCORE_EXTENSION).toBe 50
    expect(avformat.AVPROBE_SCORE_MIME).toBe 75
    expect(avformat.AVPROBE_SCORE_MAX).toBe 100
    expect(avformat.AVPROBE_PADDING_SIZE).toBe 32

  it 'avformat format constants', ->
    expect(avformat.AVFMT_NOFILE).toBe 1
    expect(avformat.AVFMT_NEEDNUMBER).toBe 2
    expect(avformat.AVFMT_RAWPICTURE).toBe 32
    expect(avformat.AVFMT_NOSTREAMS).toBe 4096
    expect(avformat.AVFMT_NO_BYTE_SEEK).toBe 32768
    expect(avformat.AVFMT_SEEK_TO_PTS).toBe 67108864

  it 'avformat AVStreamParseType enumerators', ->
    AVStreamParseType = avformat.AVStreamParseType
    expect(AVStreamParseType.AVSTREAM_PARSE_NONE).toBe 0
    expect(AVStreamParseType.AVSTREAM_PARSE_FULL).toBe 1
    expect(AVStreamParseType.AVSTREAM_PARSE_HEADERS).toBe 2
    expect(AVStreamParseType.AVSTREAM_PARSE_TIMESTAMPS).toBe 3
    expect(AVStreamParseType.AVSTREAM_PARSE_FULL_ONCE).toBe 4
    expect(AVStreamParseType.AVSTREAM_PARSE_FULL_RAW).toBe 1463898624

  it 'avformat disposition constants', ->
    expect(avformat.AV_DISPOSITION_DEFAULT).toBe 1
    expect(avformat.AV_DISPOSITION_DUB).toBe 2
    expect(avformat.AV_DISPOSITION_ORIGINAL).toBe 4
    expect(avformat.AV_DISPOSITION_ATTACHED_PIC).toBe 1024
    expect(avformat.AV_DISPOSITION_DESCRIPTIONS).toBe 131072
    expect(avformat.AV_DISPOSITION_METADATA).toBe 262144

  it 'avformat pts constants', ->
    expect(avformat.AV_PTS_WRAP_IGNORE).toBe 0
    expect(avformat.AV_PTS_WRAP_ADD_OFFSET).toBe 1
    expect(avformat.AV_PTS_WRAP_SUB_OFFSET).toBe -1

  it 'avformat AVDurationEstimationMethod enumerators', ->
    AVDurationEstimationMethod = avformat.AVDurationEstimationMethod
    expect(AVDurationEstimationMethod.AVFMT_DURATION_FROM_PTS).toBe 0
    expect(AVDurationEstimationMethod.AVFMT_DURATION_FROM_STREAM).toBe 1
    expect(AVDurationEstimationMethod.AVFMT_DURATION_FROM_BITRATE).toBe 2

  it 'avformat format flags', ->
    expect(avformat.AVFMT_FLAG_GENPTS).toBe 1
    expect(avformat.AVFMT_FLAG_IGNIDX).toBe 2
    expect(avformat.AVFMT_FLAG_NONBLOCK).toBe 4
    expect(avformat.AVFMT_FLAG_NOPARSE).toBe 32
    expect(avformat.AVFMT_FLAG_NOBUFFER).toBe 64
    expect(avformat.AVFMT_FLAG_FLUSH_PACKETS).toBe 512

  it 'avformat format avoid', ->
    expect(avformat.AVFMT_AVOID_NEG_TS_AUTO).toBe -1
    expect(avformat.AVFMT_AVOID_NEG_TS_MAKE_NON_NEGATIVE).toBe 1
    expect(avformat.AVFMT_AVOID_NEG_TS_MAKE_ZERO).toBe 2

  it 'avformat seek flags', ->
    expect(avformat.AVSEEK_FLAG_BACKWARD).toBe 1
    expect(avformat.AVSEEK_FLAG_BYTE).toBe 2
    expect(avformat.AVSEEK_FLAG_ANY).toBe 4
    expect(avformat.AVSEEK_FLAG_FRAME).toBe 8
