ffmpeg = require '../../dist/lib/ffmpeg'
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVCodecID = avcodec.AVCodecID
AVDiscard = avcodec.AVDiscard
AVAudioServiceType = avcodec.AVAudioServiceType
AVFieldOrder = avcodec.AVFieldOrder
AVSubtitleType = avcodec.AVSubtitleType
AVPictureStructure = avcodec.AVPictureStructure
AVMediaType = avutil.AVMediaType
AVSampleFormat = avutil.AVSampleFormat


describe 'avcodec', ->

  it 'test avcodec functions', ->
    expect(avcodec.version()).toBe 3676772
    expect(avcodec.license()).toBe 'LGPL version 2.1 or later'

  describe 'AVCodecID', ->

    describe 'check predefined constants', ->

      it 'codec id', ->
        expect(AVCodecID.AV_CODEC_ID_NONE).toBe 0
        expect(AVCodecID.AV_CODEC_ID_MPEG2VIDEO).toBe 2
        expect(AVCodecID.AV_CODEC_ID_H264).toBe 28
        expect(AVCodecID.AV_CODEC_ID_GIF).toBe 98
        expect(AVCodecID.AV_CODEC_ID_HEVC).toBe 1211250229
        expect(AVCodecID.AV_CODEC_ID_PCM_S8).toBe 65540
        expect(AVCodecID.AV_CODEC_ID_PCM_S24LE).toBe 65548
        expect(AVCodecID.AV_CODEC_ID_MP3).toBe 86017
        expect(AVCodecID.AV_CODEC_ID_AC3).toBe 86019
        expect(AVCodecID.AV_CODEC_ID_DTS).toBe 86020
        expect(AVCodecID.AV_CODEC_ID_FLAC).toBe 86028
        expect(AVCodecID.AV_CODEC_ID_ALAC).toBe 86032
        expect(AVCodecID.AV_CODEC_ID_TEXT).toBe 94210
        expect(AVCodecID.AV_CODEC_ID_SAMI).toBe 1396788553

    describe 'test utility functions', ->
      getBitsPerSample = AVCodecID.getBitsPerSample
      getPcmCodec = AVCodecID.getPcmCodec
      getExactBitsPerSample = AVCodecID.getExactBitsPerSample

      it '.getBitsPerSample()', ->
        expect(getBitsPerSample AVCodecID.AV_CODEC_ID_ADPCM_SBPRO_2).toBe 2
        expect(getBitsPerSample AVCodecID.AV_CODEC_ID_ADPCM_SBPRO_3).toBe 3
        expect(getBitsPerSample AVCodecID.AV_CODEC_ID_ADPCM_SBPRO_4).toBe 4
        expect(getBitsPerSample AVCodecID.AV_CODEC_ID_ADPCM_IMA_WAV).toBe 4
        expect(getBitsPerSample AVCodecID.AV_CODEC_ID_ADPCM_IMA_QT).toBe 4
        expect(getBitsPerSample AVCodecID.AV_CODEC_ID_ADPCM_SWF).toBe 4
        expect(getBitsPerSample AVCodecID.AV_CODEC_ID_ADPCM_MS).toBe 4

      it '.getPcmCodec()', ->
        expect(getPcmCodec AVSampleFormat.AV_SAMPLE_FMT_U8, 0).toBe 65541
        expect(getPcmCodec AVSampleFormat.AV_SAMPLE_FMT_U8, 1).toBe 65541

      it '.getExactBitsPerSample()', ->
        expect(getExactBitsPerSample AVCodecID.AV_CODEC_ID_ADPCM_YAMAHA).toBe 4
        expect(getExactBitsPerSample AVCodecID.AV_CODEC_ID_DSD_LSBF).toBe 8
        expect(getExactBitsPerSample AVCodecID.AV_CODEC_ID_PCM_U16BE).toBe 16
        expect(getExactBitsPerSample AVCodecID.AV_CODEC_ID_PCM_U24LE).toBe 24
        expect(getExactBitsPerSample AVCodecID.AV_CODEC_ID_PCM_S32LE).toBe 32
        expect(getExactBitsPerSample AVCodecID.AV_CODEC_ID_PCM_F64LE).toBe 64

  describe 'check avcodec defines', ->

    it 'codec property', ->
      expect(avcodec.AV_CODEC_PROP_INTRA_ONLY).toBe 1
      expect(avcodec.AV_CODEC_PROP_LOSSY).toBe 2
      expect(avcodec.AV_CODEC_PROP_LOSSLESS).toBe 4
      expect(avcodec.AV_CODEC_PROP_REORDER).toBe 8
      expect(avcodec.AV_CODEC_PROP_BITMAP_SUB).toBe 65536
      expect(avcodec.AV_CODEC_PROP_TEXT_SUB).toBe 131072

    it 'buffer size', ->
      expect(avcodec.FF_INPUT_BUFFER_PADDING_SIZE).toBe 32
      expect(avcodec.FF_MIN_BUFFER_SIZE).toBe 16384

    it 'AVDiscard', ->
      expect(AVDiscard.AVDISCARD_NONE).toBe -16
      expect(AVDiscard.AVDISCARD_DEFAULT).toBe 0
      expect(AVDiscard.AVDISCARD_NONREF).toBe 8
      expect(AVDiscard.AVDISCARD_BIDIR).toBe 16
      expect(AVDiscard.AVDISCARD_NONINTRA).toBe 24
      expect(AVDiscard.AVDISCARD_NONKEY).toBe 32
      expect(AVDiscard.AVDISCARD_ALL).toBe 48

    it 'AVAudioServiceType', ->
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_MAIN).toBe 0
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_EFFECTS).toBe 1
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_VISUALLY_IMPAIRED).toBe 2
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_HEARING_IMPAIRED).toBe 3
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_DIALOGUE).toBe 4
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_COMMENTARY).toBe 5
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_EMERGENCY).toBe 6
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_VOICE_OVER).toBe 7
      expect(AVAudioServiceType.AV_AUDIO_SERVICE_TYPE_KARAOKE).toBe 8

    it 'codec flag', ->
      expect(avcodec.CODEC_FLAG_UNALIGNED).toBe 1
      expect(avcodec.CODEC_FLAG_QSCALE).toBe 2
      expect(avcodec.CODEC_FLAG_QPEL).toBe 16
      expect(avcodec.CODEC_FLAG_CLOSED_GOP).toBe 2147483648

      expect(avcodec.CODEC_FLAG2_FAST).toBe 1
      expect(avcodec.CODEC_FLAG2_NO_OUTPUT).toBe 4
      expect(avcodec.CODEC_FLAG2_SKIP_MANUAL).toBe 536870912

    it 'codec capabilities', ->
      expect(avcodec.CODEC_CAP_TRUNCATED).toBe 8
      expect(avcodec.CODEC_CAP_HWACCEL).toBe 16
      expect(avcodec.CODEC_CAP_INTRA_ONLY).toBe 1073741824
      expect(avcodec.CODEC_CAP_LOSSLESS).toBe 2147483648

    it 'AVFieldOrder', ->
      expect(AVFieldOrder.AV_FIELD_UNKNOWN).toBe 0
      expect(AVFieldOrder.AV_FIELD_PROGRESSIVE).toBe 1
      expect(AVFieldOrder.AV_FIELD_TT).toBe 2
      expect(AVFieldOrder.AV_FIELD_BB).toBe 3
      expect(AVFieldOrder.AV_FIELD_TB).toBe 4
      expect(AVFieldOrder.AV_FIELD_BT).toBe 5

    it 'profile', ->
      expect(avcodec.FF_PROFILE_UNKNOWN).toBe -99
      expect(avcodec.FF_PROFILE_RESERVED).toBe -100
      expect(avcodec.FF_PROFILE_MPEG2_MAIN).toBe 4
      expect(avcodec.FF_PROFILE_MPEG2_SIMPLE).toBe 5
      expect(avcodec.FF_PROFILE_H264_BASELINE).toBe 66
      expect(avcodec.FF_PROFILE_H264_MAIN).toBe 77
      expect(avcodec.FF_PROFILE_H264_HIGH).toBe 100
      expect(avcodec.FF_PROFILE_HEVC_MAIN).toBe 1
      expect(avcodec.FF_PROFILE_HEVC_MAIN_10).toBe 2
      expect(avcodec.FF_PROFILE_HEVC_REXT).toBe 4

    it 'AVSubtitleType', ->
      expect(AVSubtitleType.SUBTITLE_NONE).toBe 0
      expect(AVSubtitleType.SUBTITLE_BITMAP).toBe 1
      expect(AVSubtitleType.SUBTITLE_TEXT).toBe 2
      expect(AVSubtitleType.SUBTITLE_ASS).toBe 3

    it 'AVPictureStructure', ->
      expect(AVPictureStructure.AV_PICTURE_STRUCTURE_UNKNOWN).toBe 0
      expect(AVPictureStructure.AV_PICTURE_STRUCTURE_TOP_FIELD).toBe 1
      expect(AVPictureStructure.AV_PICTURE_STRUCTURE_BOTTOM_FIELD).toBe 2
      expect(AVPictureStructure.AV_PICTURE_STRUCTURE_FRAME).toBe 3

  describe 'AVCodec', ->

    describe 'all codec list', ->
      codecs = avcodec.codecs()

      it 'total number of codecs', ->
        expect(codecs.length).toBe 513

      it 'codecs[84]', ->
        expect(codecs[84].is_encoder).toBe true
        expect(codecs[84].is_decoder).toBe false
        expect(codecs[84].name).toBe 'gif'
        expect(codecs[84].long_name).toBe 'GIF (Graphics Interchange Format)'
        expect(codecs[84].type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(codecs[84].id).toBe AVCodecID.AV_CODEC_ID_GIF
        expect(codecs[84].capabilities).toBe 0
        expect(codecs[84].supported_framerates).toEqual []
        expect(codecs[84].pix_fmts).toEqual [22, 19, 24, 21, 8, 11]
        expect(codecs[84].supported_samplerates).toEqual []
        expect(codecs[84].sample_fmts).toEqual []
        expect(codecs[84].channel_layouts).toEqual []
        expect(codecs[84].max_lowres).toBe 0
        expect(codecs[84].profiles).toEqual []

      it 'codecs[85]', ->
        expect(codecs[85].is_encoder).toBe false
        expect(codecs[85].is_decoder).toBe true
        expect(codecs[85].name).toBe 'gif'
        expect(codecs[85].long_name).toBe 'GIF (Graphics Interchange Format)'
        expect(codecs[85].type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(codecs[85].id).toBe AVCodecID.AV_CODEC_ID_GIF
        expect(codecs[85].capabilities).toBe 2
        expect(codecs[85].supported_framerates).toEqual []
        expect(codecs[85].pix_fmts).toEqual []
        expect(codecs[85].supported_samplerates).toEqual []
        expect(codecs[85].sample_fmts).toEqual []
        expect(codecs[85].channel_layouts).toEqual []
        expect(codecs[85].max_lowres).toBe 0
        expect(codecs[85].profiles).toEqual []

    describe '.findDecoder()', ->
      findDecoder = avcodec.findDecoder

      it 'find mpeg2 decoder codec', ->
        codec = findDecoder 'mpeg2video'

        expect(codec.is_encoder).toBe false
        expect(codec.is_decoder).toBe true
        expect(codec.name).toBe 'mpeg2video'
        expect(codec.long_name).toBe 'MPEG-2 video'
        expect(codec.type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(codec.id).toBe AVCodecID.AV_CODEC_ID_MPEG2VIDEO
        expect(codec.capabilities).toBe 8235
        expect(codec.supported_framerates).toEqual []
        expect(codec.pix_fmts).toEqual []
        expect(codec.supported_samplerates).toEqual []
        expect(codec.sample_fmts).toEqual []
        expect(codec.channel_layouts).toEqual []
        expect(codec.max_lowres).toBe 3
        expect(codec.profiles).toEqual [
          {profile: avcodec.FF_PROFILE_MPEG2_422, name: '4:2:2'}
          {profile: avcodec.FF_PROFILE_MPEG2_HIGH, name: 'High'}
          {profile: avcodec.FF_PROFILE_MPEG2_SS, name: 'Spatially Scalable'}
          {profile: avcodec.FF_PROFILE_MPEG2_SNR_SCALABLE, name: 'SNR Scalable'}
          {profile: avcodec.FF_PROFILE_MPEG2_MAIN, name: 'Main'}
          {profile: avcodec.FF_PROFILE_MPEG2_SIMPLE, name: 'Simple'}
          {profile: avcodec.FF_PROFILE_RESERVED, name: 'Reserved'}
          {profile: avcodec.FF_PROFILE_RESERVED, name: 'Reserved'}
        ]

      it 'find vc1 decoder codec', ->
        codec = findDecoder AVCodecID.AV_CODEC_ID_VC1

        expect(codec.is_encoder).toBe false
        expect(codec.is_decoder).toBe true
        expect(codec.name).toBe 'vc1'
        expect(codec.long_name).toBe 'SMPTE VC-1'
        expect(codec.type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(codec.id).toBe AVCodecID.AV_CODEC_ID_VC1
        expect(codec.capabilities).toBe 34
        expect(codec.supported_framerates).toEqual []
        expect(codec.pix_fmts).toEqual [0]
        expect(codec.supported_samplerates).toEqual []
        expect(codec.sample_fmts).toEqual []
        expect(codec.channel_layouts).toEqual []
        expect(codec.max_lowres).toBe 0
        expect(codec.profiles).toEqual [
          {profile: avcodec.FF_PROFILE_VC1_SIMPLE, name: 'Simple'}
          {profile: avcodec.FF_PROFILE_VC1_MAIN, name: 'Main'}
          {profile: avcodec.FF_PROFILE_VC1_COMPLEX, name: 'Complex'}
          {profile: avcodec.FF_PROFILE_VC1_ADVANCED, name: 'Advanced'}
        ]

      it 'find h264 decoder codec', ->
        codec = findDecoder 'h264'

        expect(codec.is_encoder).toBe false
        expect(codec.is_decoder).toBe true
        expect(codec.name).toBe 'h264'
        expect(codec.long_name).toBe 'H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10'
        expect(codec.type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(codec.id).toBe AVCodecID.AV_CODEC_ID_H264
        expect(codec.capabilities).toBe 12322
        expect(codec.supported_framerates).toEqual []
        expect(codec.pix_fmts).toEqual []
        expect(codec.supported_samplerates).toEqual []
        expect(codec.sample_fmts).toEqual []
        expect(codec.channel_layouts).toEqual []
        expect(codec.max_lowres).toBe 0
        expect(codec.profiles).toEqual [
          {profile: avcodec.FF_PROFILE_H264_BASELINE, name: 'Baseline'}
          {profile: avcodec.FF_PROFILE_H264_CONSTRAINED_BASELINE, name: 'Constrained Baseline'}
          {profile: avcodec.FF_PROFILE_H264_MAIN, name: 'Main'}
          {profile: avcodec.FF_PROFILE_H264_EXTENDED, name: 'Extended'}
          {profile: avcodec.FF_PROFILE_H264_HIGH, name: 'High'}
          {profile: avcodec.FF_PROFILE_H264_HIGH_10, name: 'High 10'}
          {profile: avcodec.FF_PROFILE_H264_HIGH_10_INTRA, name: 'High 10 Intra'}
          {profile: avcodec.FF_PROFILE_H264_HIGH_422, name: 'High 4:2:2'}
          {profile: avcodec.FF_PROFILE_H264_HIGH_422_INTRA, name: 'High 4:2:2 Intra'}
          {profile: avcodec.FF_PROFILE_H264_HIGH_444, name: 'High 4:4:4'}
          {profile: avcodec.FF_PROFILE_H264_HIGH_444_PREDICTIVE, name: 'High 4:4:4 Predictive'}
          {profile: avcodec.FF_PROFILE_H264_HIGH_444_INTRA, name: 'High 4:4:4 Intra'}
          {profile: avcodec.FF_PROFILE_H264_CAVLC_444, name: 'CAVLC 4:4:4'}
        ]

      it 'find hevc decoder codec', ->
        codec = findDecoder AVCodecID.AV_CODEC_ID_HEVC

        expect(codec.is_encoder).toBe false
        expect(codec.is_decoder).toBe true
        expect(codec.name).toBe 'hevc'
        expect(codec.long_name).toBe 'HEVC (High Efficiency Video Coding)'
        expect(codec.type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(codec.id).toBe AVCodecID.AV_CODEC_ID_HEVC
        expect(codec.capabilities).toBe 12322
        expect(codec.supported_framerates).toEqual []
        expect(codec.pix_fmts).toEqual []
        expect(codec.supported_samplerates).toEqual []
        expect(codec.sample_fmts).toEqual []
        expect(codec.channel_layouts).toEqual []
        expect(codec.max_lowres).toBe 0
        expect(codec.profiles).toEqual [
          {profile: avcodec.FF_PROFILE_HEVC_MAIN, name: 'Main'}
          {profile: avcodec.FF_PROFILE_HEVC_MAIN_10, name: 'Main 10'}
          {profile: avcodec.FF_PROFILE_HEVC_MAIN_STILL_PICTURE, name: 'Main Still Picture'}
          {profile: avcodec.FF_PROFILE_HEVC_REXT, name: 'Rext'}
        ]

    describe '.findEncoder()', ->
      findEncoder = avcodec.findEncoder

      it 'find mpeg4 encoder codec', ->
        codec = findEncoder 'mpeg4'

        expect(codec.is_encoder).toBe true
        expect(codec.is_decoder).toBe false
        expect(codec.name).toBe 'mpeg4'
        expect(codec.long_name).toBe 'MPEG-4 part 2'
        expect(codec.type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(codec.id).toBe AVCodecID.AV_CODEC_ID_MPEG4
        expect(codec.capabilities).toBe 8224
        expect(codec.supported_framerates).toEqual []
        expect(codec.pix_fmts).toEqual [0]
        expect(codec.supported_samplerates).toEqual []
        expect(codec.sample_fmts).toEqual []
        expect(codec.channel_layouts).toEqual []
        expect(codec.max_lowres).toBe 0
        expect(codec.profiles).toEqual []

      it 'find flac encoder codec', ->
        codec = findEncoder AVCodecID.AV_CODEC_ID_FLAC

        expect(codec.is_encoder).toBe true
        expect(codec.is_decoder).toBe false
        expect(codec.name).toBe 'flac'
        expect(codec.long_name).toBe 'FLAC (Free Lossless Audio Codec)'
        expect(codec.type).toBe AVMediaType.AVMEDIA_TYPE_AUDIO
        expect(codec.id).toBe AVCodecID.AV_CODEC_ID_FLAC
        expect(codec.capabilities).toBe -2147483552
        expect(codec.supported_framerates).toEqual []
        expect(codec.pix_fmts).toEqual []
        expect(codec.supported_samplerates).toEqual []
        expect(codec.sample_fmts).toEqual [1, 2]
        expect(codec.channel_layouts).toEqual []
        expect(codec.max_lowres).toBe 0
        expect(codec.profiles).toEqual []

    describe '.getMediaType()', ->
      getMediaType = avcodec.getMediaType

      it 'get type by codec id', ->
        expect(getMediaType AVCodecID.AV_CODEC_ID_MPEG2VIDEO).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(getMediaType AVCodecID.AV_CODEC_ID_MPEG4).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(getMediaType AVCodecID.AV_CODEC_ID_H264).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(getMediaType AVCodecID.AV_CODEC_ID_VC1).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
        expect(getMediaType AVCodecID.AV_CODEC_ID_HEVC).toBe AVMediaType.AVMEDIA_TYPE_VIDEO

    describe '.getCodecName()', ->
      getCodecName = avcodec.getCodecName

      it 'get name by codec id', ->
        expect(getCodecName AVCodecID.AV_CODEC_ID_MPEG2VIDEO).toBe 'mpeg2video'
        expect(getCodecName AVCodecID.AV_CODEC_ID_MPEG4).toBe 'mpeg4'
        expect(getCodecName AVCodecID.AV_CODEC_ID_H264).toBe 'h264'
        expect(getCodecName AVCodecID.AV_CODEC_ID_VC1).toBe 'vc1'
        expect(getCodecName AVCodecID.AV_CODEC_ID_HEVC).toBe 'hevc'
