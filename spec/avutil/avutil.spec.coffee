ffmpeg = require '../../dist/lib/ffmpeg'
avutil = ffmpeg.avutil

AVMediaType = avutil.AVMediaType
AVPictureType = avutil.AVPictureType


describe 'avutil', ->

  it 'test avutil functions', ->
    expect(avutil.version()).toBe 3544164
    expect(avutil.license()).toBe 'LGPL version 2.1 or later'
    expect(avutil.getTimeBaseQ()).toEqual num: 1, den: 1000000

  it 'check avutil defines', ->
    expect(avutil.FF_LAMBDA_SHIFT).toBe 7
    expect(avutil.FF_LAMBDA_SCALE).toBe 128
    expect(avutil.FF_QP2LAMBDA).toBe 118
    expect(avutil.FF_LAMBDA_MAX).toBe 32767
    expect(avutil.FF_QUALITY_SCALE).toBe 128
    expect(avutil.AV_NOPTS_VALUE).toBe -9223372036854776000
    expect(avutil.AV_TIME_BASE).toBe 1000000
    expect(avutil.AV_TIME_BASE_Q).toEqual num: 1, den: 1000000

  describe 'AVMediaType', ->

    describe 'check predefined constants', ->

      it 'media type', ->
        expect(AVMediaType.AVMEDIA_TYPE_UNKNOWN).toBe -1
        expect(AVMediaType.AVMEDIA_TYPE_VIDEO).toBe 0
        expect(AVMediaType.AVMEDIA_TYPE_AUDIO).toBe 1
        expect(AVMediaType.AVMEDIA_TYPE_DATA).toBe 2
        expect(AVMediaType.AVMEDIA_TYPE_SUBTITLE).toBe 3
        expect(AVMediaType.AVMEDIA_TYPE_ATTACHMENT).toBe 4

    describe 'test utility functions', ->
      getMediaTypeString = AVMediaType.getMediaTypeString

      it '.getAVMediaTypeString()', ->
        expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_UNKNOWN).toBe ''
        expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_VIDEO).toBe 'video'
        expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_AUDIO).toBe 'audio'
        expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_DATA).toBe 'data'
        expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_SUBTITLE).toBe 'subtitle'
        expect(getMediaTypeString AVMediaType.AVMEDIA_TYPE_ATTACHMENT).toBe 'attachment'

  describe 'AVPictureType', ->

    describe 'check predefined constants', ->

      it 'picture type', ->
        expect(AVPictureType.AV_PICTURE_TYPE_NONE).toBe 0
        expect(AVPictureType.AV_PICTURE_TYPE_I).toBe 1
        expect(AVPictureType.AV_PICTURE_TYPE_P).toBe 2
        expect(AVPictureType.AV_PICTURE_TYPE_B).toBe 3
        expect(AVPictureType.AV_PICTURE_TYPE_S).toBe 4
        expect(AVPictureType.AV_PICTURE_TYPE_SI).toBe 5
        expect(AVPictureType.AV_PICTURE_TYPE_SP).toBe 6
        expect(AVPictureType.AV_PICTURE_TYPE_BI).toBe 7

    describe 'test utility functions', ->
      getPictureTypeChar = AVPictureType.getPictureTypeChar

      it '.AVPictureTypeChar()', ->
        expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_NONE).toBe '?'
        expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_I).toBe 'I'
        expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_P).toBe 'P'
        expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_B).toBe 'B'
        expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_S).toBe 'S'
        expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_SI).toBe 'i'
        expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_SP).toBe 'p'
        expect(getPictureTypeChar AVPictureType.AV_PICTURE_TYPE_BI).toBe 'b'
