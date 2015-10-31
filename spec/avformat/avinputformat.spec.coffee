ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec

AVCodecID = avcodec.AVCodecID


describe 'AVInputFormat', ->

  describe 'all input format list', ->
    iformats = avformat.iformats()

    it 'total number of input formats', ->
      expect(iformats.length).toBe 234

    it 'iformats[15]', ->
      expect(iformats[15]).toEqual {
        name: 'asf'
        long_name: 'ASF (Advanced / Active Streaming Format)'
        flags: 24576
        extensions: ''
        mime_type: ''
        raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
      }

    it 'iformats[19]', ->
      expect(iformats[19]).toEqual {
        name: 'avi'
        long_name: 'AVI (Audio Video Interleaved)'
        flags: 0
        extensions: 'avi'
        mime_type: ''
        raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
      }

    it 'iformats[71]', ->
      expect(iformats[71]).toEqual {
        name: 'h264'
        long_name: 'raw H.264 video'
        flags: 256
        extensions: 'h26l,h264,264,avc'
        mime_type: ''
        raw_codec_id: AVCodecID.AV_CODEC_ID_H264
      }

    it 'iformats[99]', ->
      expect(iformats[99]).toEqual {
        name: 'matroska,webm'
        long_name: 'Matroska / WebM'
        flags: 0
        extensions: 'mkv,mk3d,mka,mks'
        mime_type: 'audio/webm,audio/x-matroska,video/webm,video/x-matroska'
        raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
      }

  describe '.findInputFormat()', ->
    findInputFormat = avformat.findInputFormat

    it 'find mp4 input format', ->
      expect(findInputFormat 'mp4').toEqual {
        name: 'mov,mp4,m4a,3gp,3g2,mj2'
        long_name: 'QuickTime / MOV'
        flags: 32768
        extensions: 'mov,mp4,m4a,3gp,3g2,mj2'
        mime_type: ''
        raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
      }

    it 'find h264 input format', ->
      expect(findInputFormat 'h264').toEqual {
        name: 'h264'
        long_name: 'raw H.264 video'
        flags: 256
        extensions: 'h26l,h264,264,avc'
        mime_type: ''
        raw_codec_id: AVCodecID.AV_CODEC_ID_H264
      }

    it 'find hevc input format', ->
      expect(findInputFormat 'hevc').toEqual {
        name: 'hevc'
        long_name: 'raw HEVC video'
        flags: 256
        extensions: 'hevc,h265,265'
        mime_type: ''
        raw_codec_id: AVCodecID.AV_CODEC_ID_HEVC
      }
