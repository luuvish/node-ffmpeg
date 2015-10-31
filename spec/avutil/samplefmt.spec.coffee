ffmpeg = require '../../dist/lib/ffmpeg'
avutil = ffmpeg.avutil

AVSampleFormat = avutil.AVSampleFormat


describe 'AVSampleFormat', ->

  describe 'check predefined constants', ->

    it 'audio sample format', ->
      expect(AVSampleFormat.AV_SAMPLE_FMT_NONE).toBe -1
      expect(AVSampleFormat.AV_SAMPLE_FMT_U8).toBe 0
      expect(AVSampleFormat.AV_SAMPLE_FMT_S16).toBe 1
      expect(AVSampleFormat.AV_SAMPLE_FMT_S32).toBe 2
      expect(AVSampleFormat.AV_SAMPLE_FMT_FLT).toBe 3
      expect(AVSampleFormat.AV_SAMPLE_FMT_DBL).toBe 4
      expect(AVSampleFormat.AV_SAMPLE_FMT_U8P).toBe 5
      expect(AVSampleFormat.AV_SAMPLE_FMT_S16P).toBe 6
      expect(AVSampleFormat.AV_SAMPLE_FMT_S32P).toBe 7
      expect(AVSampleFormat.AV_SAMPLE_FMT_FLTP).toBe 8
      expect(AVSampleFormat.AV_SAMPLE_FMT_DBLP).toBe 9

  describe 'test utility functions', ->
    getSampleFmtName = AVSampleFormat.getSampleFmtName
    getSampleFmt = AVSampleFormat.getSampleFmt
    getAltSampleFmt = AVSampleFormat.getAltSampleFmt
    getPackedSampleFmt = AVSampleFormat.getPackedSampleFmt
    getPlanarSampleFmt = AVSampleFormat.getPlanarSampleFmt
    getBytesPerSample = AVSampleFormat.getBytesPerSample
    isSampleFmtPlanar = AVSampleFormat.isSampleFmtPlanar
    getSamplesBufferSize = AVSampleFormat.getSamplesBufferSize

    it '.getSampleFmtName()', ->
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_U8).toBe 'u8'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_S16).toBe 's16'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_S32).toBe 's32'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_FLT).toBe 'flt'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_DBL).toBe 'dbl'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_U8P).toBe 'u8p'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_S16P).toBe 's16p'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_S32P).toBe 's32p'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_FLTP).toBe 'fltp'
      expect(getSampleFmtName AVSampleFormat.AV_SAMPLE_FMT_DBLP).toBe 'dblp'

    it '.getSampleFmt()', ->
      expect(getSampleFmt 'u8').toBe AVSampleFormat.AV_SAMPLE_FMT_U8
      expect(getSampleFmt 's16').toBe AVSampleFormat.AV_SAMPLE_FMT_S16
      expect(getSampleFmt 's32').toBe AVSampleFormat.AV_SAMPLE_FMT_S32
      expect(getSampleFmt 'flt').toBe AVSampleFormat.AV_SAMPLE_FMT_FLT
      expect(getSampleFmt 'dbl').toBe AVSampleFormat.AV_SAMPLE_FMT_DBL
      expect(getSampleFmt 'u8p').toBe AVSampleFormat.AV_SAMPLE_FMT_U8P
      expect(getSampleFmt 's16p').toBe AVSampleFormat.AV_SAMPLE_FMT_S16P
      expect(getSampleFmt 's32p').toBe AVSampleFormat.AV_SAMPLE_FMT_S32P
      expect(getSampleFmt 'fltp').toBe AVSampleFormat.AV_SAMPLE_FMT_FLTP
      expect(getSampleFmt 'dblp').toBe AVSampleFormat.AV_SAMPLE_FMT_DBLP

    it '.getAltSampleFmt()', ->
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_U8, 1).toBe AVSampleFormat.AV_SAMPLE_FMT_U8P
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_S16, 1).toBe AVSampleFormat.AV_SAMPLE_FMT_S16P
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_S32, 1).toBe AVSampleFormat.AV_SAMPLE_FMT_S32P
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_FLT, 1).toBe AVSampleFormat.AV_SAMPLE_FMT_FLTP
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_DBL, 1).toBe AVSampleFormat.AV_SAMPLE_FMT_DBLP
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_U8P, 0).toBe AVSampleFormat.AV_SAMPLE_FMT_U8
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_S16P, 0).toBe AVSampleFormat.AV_SAMPLE_FMT_S16
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_S32P, 0).toBe AVSampleFormat.AV_SAMPLE_FMT_S32
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_FLTP, 0).toBe AVSampleFormat.AV_SAMPLE_FMT_FLT
      expect(getAltSampleFmt AVSampleFormat.AV_SAMPLE_FMT_DBLP, 0).toBe AVSampleFormat.AV_SAMPLE_FMT_DBL

    it '.getPackedSampleFmt()', ->
      expect(getPackedSampleFmt AVSampleFormat.AV_SAMPLE_FMT_U8P).toBe AVSampleFormat.AV_SAMPLE_FMT_U8
      expect(getPackedSampleFmt AVSampleFormat.AV_SAMPLE_FMT_S16P).toBe AVSampleFormat.AV_SAMPLE_FMT_S16
      expect(getPackedSampleFmt AVSampleFormat.AV_SAMPLE_FMT_S32P).toBe AVSampleFormat.AV_SAMPLE_FMT_S32
      expect(getPackedSampleFmt AVSampleFormat.AV_SAMPLE_FMT_FLTP).toBe AVSampleFormat.AV_SAMPLE_FMT_FLT
      expect(getPackedSampleFmt AVSampleFormat.AV_SAMPLE_FMT_DBLP).toBe AVSampleFormat.AV_SAMPLE_FMT_DBL

    it '.getPlanarSampleFmt()', ->
      expect(getPlanarSampleFmt AVSampleFormat.AV_SAMPLE_FMT_U8).toBe AVSampleFormat.AV_SAMPLE_FMT_U8P
      expect(getPlanarSampleFmt AVSampleFormat.AV_SAMPLE_FMT_S16).toBe AVSampleFormat.AV_SAMPLE_FMT_S16P
      expect(getPlanarSampleFmt AVSampleFormat.AV_SAMPLE_FMT_S32).toBe AVSampleFormat.AV_SAMPLE_FMT_S32P
      expect(getPlanarSampleFmt AVSampleFormat.AV_SAMPLE_FMT_FLT).toBe AVSampleFormat.AV_SAMPLE_FMT_FLTP
      expect(getPlanarSampleFmt AVSampleFormat.AV_SAMPLE_FMT_DBL).toBe AVSampleFormat.AV_SAMPLE_FMT_DBLP

    it '.getBytesPerSample()', ->
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_U8).toBe 1
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_S16).toBe 2
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_S32).toBe 4
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_FLT).toBe 4
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_DBL).toBe 8
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_U8P).toBe 1
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_S16P).toBe 2
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_S32P).toBe 4
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_FLTP).toBe 4
      expect(getBytesPerSample AVSampleFormat.AV_SAMPLE_FMT_DBLP).toBe 8

    it '.isSampleFmtPlanar()', ->
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_U8).toBe false
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_S16).toBe false
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_S32).toBe false
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_FLT).toBe false
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_DBL).toBe false
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_U8P).toBe true
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_S16P).toBe true
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_S32P).toBe true
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_FLTP).toBe true
      expect(isSampleFmtPlanar AVSampleFormat.AV_SAMPLE_FMT_DBLP).toBe true

    it '.getSamplesBufferSize()', ->
      expect(getSamplesBufferSize 2, 16, AVSampleFormat.AV_SAMPLE_FMT_U8, 0).toEqual [64, 64]
      expect(getSamplesBufferSize 2, 16, AVSampleFormat.AV_SAMPLE_FMT_U8, 1).toEqual [32, 32]
