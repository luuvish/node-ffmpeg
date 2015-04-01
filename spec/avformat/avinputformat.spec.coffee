ffmpeg = require '../../lib/ffmpeg'

describe 'AVInputFormat', ->
  AVCodecID = ffmpeg.avcodec.AVCodecID

  it 'iformat list', ->
    iformats = ffmpeg.avformat.iformats()

    expect(iformats.length).toBe 234

    expect(iformats[15]).toEqual {
      name: 'asf'
      long_name: 'ASF (Advanced / Active Streaming Format)'
      flags: 24576
      extensions: ''
      mime_type: ''
      raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
    }
    expect(iformats[19]).toEqual {
      name: 'avi'
      long_name: 'AVI (Audio Video Interleaved)'
      flags: 0
      extensions: 'avi'
      mime_type: ''
      raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
    }
    expect(iformats[71]).toEqual {
      name: 'h264'
      long_name: 'raw H.264 video'
      flags: 256
      extensions: 'h26l,h264,264,avc'
      mime_type: ''
      raw_codec_id: AVCodecID.AV_CODEC_ID_H264
    }
    expect(iformats[99]).toEqual {
      name: 'matroska,webm'
      long_name: 'Matroska / WebM'
      flags: 0
      extensions: 'mkv,mk3d,mka,mks'
      mime_type: 'audio/webm,audio/x-matroska,video/webm,video/x-matroska'
      raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
    }

  it 'find mp4', ->
    fmt = ffmpeg.avformat.findInputFormat 'mp4'

    expect(fmt).toEqual {
      name: 'mov,mp4,m4a,3gp,3g2,mj2'
      long_name: 'QuickTime / MOV'
      flags: 32768
      extensions: 'mov,mp4,m4a,3gp,3g2,mj2'
      mime_type: ''
      raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
    }

  it 'find h264', ->
    fmt = ffmpeg.avformat.findInputFormat 'h264'

    expect(fmt).toEqual {
      name: 'h264'
      long_name: 'raw H.264 video'
      flags: 256
      extensions: 'h26l,h264,264,avc'
      mime_type: ''
      raw_codec_id: AVCodecID.AV_CODEC_ID_H264
    }

  it 'find hevc', ->
    fmt = ffmpeg.avformat.findInputFormat 'hevc'

    expect(fmt).toEqual {
      name: 'hevc'
      long_name: 'raw HEVC video'
      flags: 256
      extensions: 'hevc,h265,265'
      mime_type: ''
      raw_codec_id: AVCodecID.AV_CODEC_ID_HEVC
    }
