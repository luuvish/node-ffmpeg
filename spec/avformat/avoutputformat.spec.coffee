ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVCodecID = avcodec.AVCodecID
AVMediaType = avutil.AVMediaType


describe 'AVOutputFormat', ->

  describe 'all output format list', ->
    oformats = avformat.oformats()

    it 'total number of output formats', ->
      expect(oformats.length).toBe 140

    it 'oformats[11]', ->
      expect(oformats[11]).toEqual {
        name: 'avi'
        long_name: 'AVI (Audio Video Interleaved)'
        mime_type: 'video/x-msvideo'
        extensions: 'avi'
        audio_codec: AVCodecID.AV_CODEC_ID_AC3
        video_codec: AVCodecID.AV_CODEC_ID_MPEG4
        subtitle_codec: AVCodecID.AV_CODEC_ID_NONE
        flags: 0
      }

    it 'oformats[39]', ->
      expect(oformats[39]).toEqual {
        name: 'h264'
        long_name: 'raw H.264 video'
        mime_type: ''
        extensions: 'h264,264'
        audio_codec: AVCodecID.AV_CODEC_ID_NONE
        video_codec: AVCodecID.AV_CODEC_ID_H264
        subtitle_codec: AVCodecID.AV_CODEC_ID_NONE
        flags: 128
      }

    it 'oformats[56]', ->
      expect(oformats[56]).toEqual {
        name: 'matroska'
        long_name: 'Matroska'
        mime_type: 'video/x-matroska'
        extensions: 'mkv'
        audio_codec: AVCodecID.AV_CODEC_ID_AC3
        video_codec: AVCodecID.AV_CODEC_ID_MPEG4
        subtitle_codec: AVCodecID.AV_CODEC_ID_ASS
        flags: 197696
      }

    it 'oformats[65]', ->
      expect(oformats[65]).toEqual {
        name: 'mp4'
        long_name: 'MP4 (MPEG-4 Part 14)'
        mime_type: 'application/mp4'
        extensions: 'mp4'
        audio_codec: AVCodecID.AV_CODEC_ID_AAC
        video_codec: AVCodecID.AV_CODEC_ID_MPEG4
        subtitle_codec: AVCodecID.AV_CODEC_ID_NONE
        flags: 327744
      }

  describe 'test utility functions', ->
    guessFormat = avformat.guessFormat
    guessCodec = avformat.guessCodec
    queryCodec = avformat.queryCodec

    describe '.guessFormat()', ->

      it 'guess webm output format', ->
        expect(guessFormat 'webm', 'WebM', 'video/webm').toEqual {
          name: 'webm'
          long_name: 'WebM'
          mime_type: 'video/webm'
          extensions: 'webm'
          audio_codec: AVCodecID.AV_CODEC_ID_VORBIS
          video_codec: AVCodecID.AV_CODEC_ID_VP8
          subtitle_codec: AVCodecID.AV_CODEC_ID_WEBVTT
          flags: 197696
        }

      it 'guess vc1 output format', ->
        expect(guessFormat 'vc1', '', '').toEqual {
          name: 'vc1'
          long_name: 'raw VC-1 video'
          mime_type: ''
          extensions: 'vc1'
          audio_codec: AVCodecID.AV_CODEC_ID_NONE
          video_codec: AVCodecID.AV_CODEC_ID_VC1
          subtitle_codec: AVCodecID.AV_CODEC_ID_NONE
          flags: 128
        }

    describe '.guessCodec()', ->

      it 'guess vc1 codec', ->
        fmt = guessFormat 'vc1', '', ''
        expect(guessCodec fmt, 'vc1', '', '', 0).toBe AVCodecID.AV_CODEC_ID_VC1

    describe '.queryCodec()', ->

      it 'query vc1 codec', ->
        fmt = guessFormat 'vc1', '', ''
        expect(queryCodec fmt, AVCodecID.AV_CODEC_ID_VC1, AVMediaType.AVMEDIA_TYPE_VIDEO).toBe 1
