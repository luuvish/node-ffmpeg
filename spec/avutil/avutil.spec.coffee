ffmpeg = require '../../lib/ffmpeg'

describe 'ffmpeg.avutil', ->
  avutil = ffmpeg.avutil

  it 'avutil functions', ->
    expect(avutil.version()).toBe 3544164
    expect(avutil.license()).toBe 'LGPL version 2.1 or later'
    expect(avutil.getTimeBaseQ()).toEqual { num: 1, den: 1000000 }

  it 'avutil constants', ->
    expect(avutil.FF_LAMBDA_SHIFT).toBe 7
    expect(avutil.FF_LAMBDA_SCALE).toBe 128
    expect(avutil.FF_QP2LAMBDA).toBe 118
    expect(avutil.FF_LAMBDA_MAX).toBe 32767
    expect(avutil.FF_QUALITY_SCALE).toBe 128
    expect(avutil.AV_NOPTS_VALUE).toBe -9223372036854776000
    expect(avutil.AV_TIME_BASE).toBe 1000000
    expect(avutil.AV_TIME_BASE_Q).toEqual { num: 1, den: 1000000 }

describe 'AVMediaType constants', ->
  AVMediaType = ffmpeg.avutil.AVMediaType

  it 'AVMediaType format', ->
    expect(AVMediaType.AVMEDIA_TYPE_UNKNOWN).toBe -1
    expect(AVMediaType.AVMEDIA_TYPE_VIDEO).toBe 0
    expect(AVMediaType.AVMEDIA_TYPE_AUDIO).toBe 1
    expect(AVMediaType.AVMEDIA_TYPE_DATA).toBe 2
    expect(AVMediaType.AVMEDIA_TYPE_SUBTITLE).toBe 3
    expect(AVMediaType.AVMEDIA_TYPE_ATTACHMENT).toBe 4

  it 'AVMediaType string', ->
    getMediaTypeString = AVMediaType.getMediaTypeString

    expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_UNKNOWN).toBe ''
    expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_VIDEO).toBe 'video'
    expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_AUDIO).toBe 'audio'
    expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_DATA).toBe 'data'
    expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_SUBTITLE).toBe 'subtitle'
    expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_ATTACHMENT).toBe 'attachment'

describe 'AVPictureType constants', ->
  AVPictureType = ffmpeg.avutil.AVPictureType

  it 'AVPictureType format', ->
    expect(AVPictureType.AV_PICTURE_TYPE_NONE).toBe 0
    expect(AVPictureType.AV_PICTURE_TYPE_I).toBe 1
    expect(AVPictureType.AV_PICTURE_TYPE_P).toBe 2
    expect(AVPictureType.AV_PICTURE_TYPE_B).toBe 3
    expect(AVPictureType.AV_PICTURE_TYPE_S).toBe 4
    expect(AVPictureType.AV_PICTURE_TYPE_SI).toBe 5
    expect(AVPictureType.AV_PICTURE_TYPE_SP).toBe 6
    expect(AVPictureType.AV_PICTURE_TYPE_BI).toBe 7

  it 'AVPictureType string', ->
    getPictureTypeChar = AVPictureType.getPictureTypeChar

    expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_NONE).toBe '?'
    expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_I).toBe 'I'
    expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_P).toBe 'P'
    expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_B).toBe 'B'
    expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_S).toBe 'S'
    expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_SI).toBe 'i'
    expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_SP).toBe 'p'
    expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_BI).toBe 'b'
