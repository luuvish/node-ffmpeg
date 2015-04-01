ffmpeg = require '../../lib/ffmpeg'

describe 'AVOutputFormat', ->
  AVCodecID = ffmpeg.avcodec.AVCodecID
  AVMediaType = ffmpeg.avutil.AVMediaType

  it 'oformat list', ->
    oformats = ffmpeg.avformat.oformats()

    expect(oformats.length).toBe 140

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

  it 'guess webm', ->
    fmt = ffmpeg.avformat.guessFormat 'webm', 'WebM', 'video/webm'

    expect(fmt).toEqual {
      name: 'webm'
      long_name: 'WebM'
      mime_type: 'video/webm'
      extensions: 'webm'
      audio_codec: AVCodecID.AV_CODEC_ID_VORBIS
      video_codec: AVCodecID.AV_CODEC_ID_VP8
      subtitle_codec: AVCodecID.AV_CODEC_ID_WEBVTT
      flags: 197696
    }

  it 'guess vc1', ->
    fmt = ffmpeg.avformat.guessFormat 'vc1', '', ''

    expect(fmt).toEqual {
      name: 'vc1'
      long_name: 'raw VC-1 video'
      mime_type: ''
      extensions: 'vc1'
      audio_codec: AVCodecID.AV_CODEC_ID_NONE
      video_codec: AVCodecID.AV_CODEC_ID_VC1
      subtitle_codec: AVCodecID.AV_CODEC_ID_NONE
      flags: 128
    }

  it 'guessCodec(), queryCodec()', ->
    fmt = ffmpeg.avformat.guessFormat 'vc1', '', ''
    guessCodec = ffmpeg.avformat.guessCodec
    queryCodec = ffmpeg.avformat.queryCodec

    expect(guessCodec fmt, 'vc1', '', '', 0).toBe AVCodecID.AV_CODEC_ID_VC1
    expect(queryCodec fmt, AVCodecID.AV_CODEC_ID_VC1, AVMediaType.AVMEDIA_TYPE_VIDEO).toBe 1
