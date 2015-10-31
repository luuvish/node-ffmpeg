ffmpeg = require '../../dist/lib/ffmpeg'
avformat = ffmpeg.avformat
avcodec = ffmpeg.avcodec
avutil = ffmpeg.avutil

AVFormatContext = avformat.AVFormatContext

AVCodecID = avcodec.AVCodecID
AVMediaType = avutil.AVMediaType


describe 'AVFormatContext', ->

  describe 'open input AVFormatContext', ->
    ctx = null

    beforeEach ->
      ctx = new AVFormatContext

    afterEach ->
      ctx = null

    it 'open mkv format context', ->
      ctx.openInput './examples/ticket-a_aac.mkv'

      expect(ctx.iformat).toEqual {
        name: 'matroska,webm'
        long_name: 'Matroska / WebM'
        flags: 0
        extensions: 'mkv,mk3d,mka,mks'
        mime_type: 'audio/webm,audio/x-matroska,video/webm,video/x-matroska'
        raw_codec_id: 0
      }
      expect(ctx.oformat).toBe null
      expect(ctx.ctx_flags).toBe 0
      expect(ctx.streams.length).toBe 5
      expect(ctx.filename).toBe './examples/ticket-a_aac.mkv'
      expect(ctx.start_time).toBe avutil.AV_NOPTS_VALUE
      expect(ctx.duration).toBe 50006000
      expect(ctx.bit_rate).toBe 0
      expect(ctx.packet_size).toBe 0
      expect(ctx.max_delay).toBe -1
      expect(ctx.flags).toBe avformat.AVFMT_FLAG_FLUSH_PACKETS
      expect(ctx.programs).toEqual []
      expect(ctx.video_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.audio_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.subtitle_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.max_index_size).toBe 1048576
      expect(ctx.max_picture_buffer).toBe 3041280
      expect(ctx.chapters).toEqual [
        id: 131881663
        time_base: {num: 1, den: 1000000000}
        start: 0
        end: avutil.AV_NOPTS_VALUE
        metadata: {title: '01. Générique de début'}
      ]
      expect(ctx.metadata).toEqual {
        title: 'Un ticket pour l\'espace'
        encoder: 'libebml v0.7.7 + libmatroska v0.8.0'
        creation_time: '2006-11-10 13:52:03'
      }
      expect(ctx.start_time_realtime).toBe avutil.AV_NOPTS_VALUE
      expect(ctx.fps_probe_size).toBe -1
      expect(ctx.error_recognition).toBe 1
      expect(ctx.max_interleave_delta).toBe 10000000
      expect(ctx.strict_std_compliance).toBe 0
      expect(ctx.event_flags).toBe 0
      expect(ctx.max_ts_probe).toBe 50
      expect(ctx.avoid_negative_ts).toBe avformat.AVFMT_AVOID_NEG_TS_AUTO
      expect(ctx.audio_preload).toBe 0
      expect(ctx.max_chunk_duration).toBe 0
      expect(ctx.max_chunk_size).toBe 0
      expect(ctx.use_wallclock_as_timestamps).toBe 0
      expect(ctx.avio_flags).toBe 0
      expect(ctx.duration_estimation_method).toBe 0
      expect(ctx.skip_initial_bytes).toBe 0
      expect(ctx.correct_ts_overflow).toBe 1
      expect(ctx.seek2any).toBe 0
      expect(ctx.flush_packets).toBe 1
      expect(ctx.probe_score).toBe 100
      expect(ctx.format_probesize).toBe 1048576
      expect(ctx.codec_whitelist).toBe ''
      expect(ctx.format_whitelist).toBe ''
      expect(ctx.io_repositioned).toBe 0
      expect(ctx.video_codec).toBe null
      expect(ctx.audio_codec).toBe null
      expect(ctx.subtitle_codec).toBe null
      expect(ctx.data_codec).toBe null
      expect(ctx.metadata_header_padding).toBe -1
      expect(ctx.output_ts_offset).toBe 0
      expect(ctx.max_analyze_duration2).toBe 0
      expect(ctx.probesize2).toBe 5000000
      expect(ctx.dump_separator).toBe ', '
      expect(ctx.data_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE

      ctx.closeInput()

    it 'open mp4 format context', ->
      fmt = avformat.findInputFormat 'mp4'
      ctx.openInput './examples/mpeg4conformance.mp4', fmt

      expect(ctx.iformat).toEqual {
        name: 'mov,mp4,m4a,3gp,3g2,mj2'
        long_name: 'QuickTime / MOV'
        flags: 32768
        extensions: 'mov,mp4,m4a,3gp,3g2,mj2'
        mime_type: ''
        raw_codec_id: AVCodecID.AV_CODEC_ID_NONE
      }
      expect(ctx.oformat).toBe null
      expect(ctx.ctx_flags).toBe 0
      expect(ctx.streams.length).toBe 3
      expect(ctx.filename).toBe './examples/mpeg4conformance.mp4'
      expect(ctx.start_time).toBe avutil.AV_NOPTS_VALUE
      expect(ctx.duration).toBe 54900000
      expect(ctx.bit_rate).toBe 0
      expect(ctx.packet_size).toBe 0
      expect(ctx.max_delay).toBe -1
      expect(ctx.flags).toBe 512
      expect(ctx.programs).toEqual []
      expect(ctx.video_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.audio_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.subtitle_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.max_index_size).toBe 1048576
      expect(ctx.max_picture_buffer).toBe 3041280
      expect(ctx.chapters).toEqual []
      expect(ctx.metadata).toEqual {
        creation_time: '2009-01-06 21:59:04'
        compatible_brands: 'mp41'
        minor_version: '0'
        major_brand: 'isom'
      }
      expect(ctx.start_time_realtime).toEqual avutil.AV_NOPTS_VALUE
      expect(ctx.fps_probe_size).toBe -1
      expect(ctx.error_recognition).toBe 1
      expect(ctx.max_interleave_delta).toBe 10000000
      expect(ctx.strict_std_compliance).toBe 0
      expect(ctx.event_flags).toBe 0
      expect(ctx.max_ts_probe).toBe 50
      expect(ctx.avoid_negative_ts).toBe -1
      expect(ctx.audio_preload).toBe 0
      expect(ctx.max_chunk_duration).toBe 0
      expect(ctx.max_chunk_size).toBe 0
      expect(ctx.use_wallclock_as_timestamps).toBe 0
      expect(ctx.avio_flags).toBe 0
      expect(ctx.duration_estimation_method).toBe 0
      expect(ctx.skip_initial_bytes).toBe 0
      expect(ctx.correct_ts_overflow).toBe 1
      expect(ctx.seek2any).toBe 0
      expect(ctx.flush_packets).toBe 1
      expect(ctx.probe_score).toBe 0
      expect(ctx.format_probesize).toBe 1048576
      expect(ctx.codec_whitelist).toBe ''
      expect(ctx.format_whitelist).toBe ''
      expect(ctx.io_repositioned).toBe 0
      expect(ctx.video_codec).toBe null
      expect(ctx.audio_codec).toBe null
      expect(ctx.subtitle_codec).toBe null
      expect(ctx.data_codec).toBe null
      expect(ctx.metadata_header_padding).toBe -1
      expect(ctx.output_ts_offset).toBe 0
      expect(ctx.max_analyze_duration2).toBe 0
      expect(ctx.probesize2).toBe 5000000
      expect(ctx.dump_separator).toBe ', '
      expect(ctx.data_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE

      ctx.closeInput()

  describe 'create output AVFormatContext', ->
    ctx = null

    beforeEach ->
      ctx = new AVFormatContext

    afterEach ->
      ctx = null

    it 'create hevc format context', ->
      ctx.openOutput 'hevc', 'output.hevc'

      expect(ctx.iformat).toBe null
      expect(ctx.oformat).toEqual {
        name: 'hevc'
        long_name: 'raw HEVC video'
        mime_type: ''
        extensions: 'hevc'
        audio_codec: AVCodecID.AV_CODEC_ID_NONE
        video_codec: AVCodecID.AV_CODEC_ID_HEVC
        subtitle_codec: AVCodecID.AV_CODEC_ID_NONE
        flags: 128
      }
      expect(ctx.ctx_flags).toBe 0
      expect(ctx.streams).toEqual []
      expect(ctx.filename).toBe 'output.hevc'
      expect(ctx.start_time).toBe 0
      expect(ctx.duration).toBe 0
      expect(ctx.bit_rate).toBe 0
      expect(ctx.packet_size).toBe 0
      expect(ctx.max_delay).toBe -1
      expect(ctx.flags).toBe avformat.AVFMT_FLAG_FLUSH_PACKETS
      expect(ctx.programs).toEqual []
      expect(ctx.video_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.audio_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.subtitle_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.max_index_size).toBe 1048576
      expect(ctx.max_picture_buffer).toBe 3041280
      expect(ctx.chapters).toEqual []
      expect(ctx.metadata).toEqual {}
      expect(ctx.start_time_realtime).toBe avutil.AV_NOPTS_VALUE
      expect(ctx.fps_probe_size).toBe -1
      expect(ctx.error_recognition).toBe 1
      expect(ctx.max_interleave_delta).toBe 10000000
      expect(ctx.strict_std_compliance).toBe 0
      expect(ctx.event_flags).toBe 0
      expect(ctx.max_ts_probe).toBe 50
      expect(ctx.avoid_negative_ts).toBe avformat.AVFMT_AVOID_NEG_TS_AUTO
      expect(ctx.audio_preload).toBe 0
      expect(ctx.max_chunk_duration).toBe 0
      expect(ctx.max_chunk_size).toBe 0
      expect(ctx.use_wallclock_as_timestamps).toBe 0
      expect(ctx.avio_flags).toBe 0
      expect(ctx.duration_estimation_method).toBe 0
      expect(ctx.skip_initial_bytes).toBe 0
      expect(ctx.correct_ts_overflow).toBe 1
      expect(ctx.seek2any).toBe 0
      expect(ctx.flush_packets).toBe 1
      expect(ctx.probe_score).toBe 0
      expect(ctx.format_probesize).toBe 1048576
      expect(ctx.codec_whitelist).toBe ''
      expect(ctx.format_whitelist).toBe ''
      expect(ctx.io_repositioned).toBe 0
      expect(ctx.video_codec).toBe null
      expect(ctx.audio_codec).toBe null
      expect(ctx.subtitle_codec).toBe null
      expect(ctx.data_codec).toBe null
      expect(ctx.metadata_header_padding).toBe -1
      expect(ctx.output_ts_offset).toBe 0
      expect(ctx.max_analyze_duration2).toBe 0
      expect(ctx.probesize2).toBe 5000000
      expect(ctx.dump_separator).toBe ', '
      expect(ctx.data_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE

      ctx.closeOutput()

  describe 'test method functions', ->
    ctx = null

    beforeEach ->
      ctx = new AVFormatContext

    afterEach ->
      ctx = null

    it '.findStreamInfo()', ->
    it '.findProgramFromStream()', ->
    it '.findBestStream()', ->
    it '.readFrame()', ->
    it '.seekFrame()', ->
    it '.seekFile()', ->
    it '.flush()', ->
    it '.readPlay()', ->
    it '.readPause()', ->
    it '.writeHeader()', ->
    it '.writeFrame()', ->
    it '.writeInterleavedFrame()', ->
    it '.writeUncodedFrame()', ->
    it '.writeInterleavedUncodedFrame()', ->
    it '.writeUncodedFrameQuery()', ->
    it '.writeTrailer()', ->
    it '.getOutputTimestamp()', ->
    it '.findDefaultStreamIndex()', ->
    it '.dumpFormat()', ->
    it '.guessSampleAspectRatio()', ->
    it '.guessFrameRate()', ->
    it '.matchStreamSpecifier()', ->
    it '.queueAttachedPictures()', ->
    it '.injectGlobalSideData()', ->

  describe 'test property getter/setter', ->
    ctx = null

    beforeEach ->
      ctx = new AVFormatContext

    afterEach ->
      ctx = null

    it '.oformat', ->
      ctx.oformat = avformat.guessFormat 'h264', '', ''
      expect(ctx.oformat).toEqual {
        name: 'h264'
        long_name: 'raw H.264 video'
        mime_type: ''
        extensions: 'h264,264'
        audio_codec: AVCodecID.AV_CODEC_ID_NONE
        video_codec: AVCodecID.AV_CODEC_ID_H264
        subtitle_codec: AVCodecID.AV_CODEC_ID_NONE
        flags: 128
      }

    it '.filename', ->
      ctx.filename = 'test.h264'
      expect(ctx.filename).toBe 'test.h264'

    it '.flags', ->
      ctx.flags = 71
      expect(ctx.flags).toBe 71

    it '.video_codec_id', ->
      ctx.video_codec_id = AVCodecID.AV_CODEC_ID_H264
      expect(ctx.video_codec_id).toBe AVCodecID.AV_CODEC_ID_H264

    it '.audio_codec_id', ->
      ctx.audio_codec_id = AVCodecID.AV_CODEC_ID_AC3
      expect(ctx.audio_codec_id).toBe AVCodecID.AV_CODEC_ID_AC3

    it '.subtitle_codec_id', ->
      ctx.subtitle_codec_id = AVCodecID.AV_CODEC_ID_ASS
      expect(ctx.subtitle_codec_id).toBe AVCodecID.AV_CODEC_ID_ASS

    it '.max_index_size', ->
      ctx.max_index_size = 1000
      expect(ctx.max_index_size).toBe 1000

    it '.start_time_realtime', ->
      ctx.start_time_realtime = 2000
      expect(ctx.start_time_realtime).toBe 2000

    it '.fps_probe_size', ->
      ctx.fps_probe_size = 12
      expect(ctx.fps_probe_size).toBe 12

    it '.error_recognition', ->
      ctx.error_recognition = 11
      expect(ctx.error_recognition).toBe 11

    it '.max_interleave_delta', ->
      ctx.max_interleave_delta = 400000
      expect(ctx.max_interleave_delta).toBe 400000

    it '.strict_std_compliance', ->
      ctx.strict_std_compliance = 2
      expect(ctx.strict_std_compliance).toBe 2

    it '.event_flags', ->
      ctx.event_flags = 5
      expect(ctx.event_flags).toBe 5

    it '.max_ts_probe', ->
      ctx.max_ts_probe = 60
      expect(ctx.max_ts_probe).toBe 60

    it '.avoid_negative_ts', ->
      ctx.avoid_negative_ts = 8
      expect(ctx.avoid_negative_ts).toBe 8

    it '.video_codec', ->
      ctx.video_codec = avcodec.findEncoder 'mpeg4'
      expect(ctx.video_codec.is_encoder).toBe true
      expect(ctx.video_codec.is_decoder).toBe false
      expect(ctx.video_codec.name).toBe 'mpeg4'
      expect(ctx.video_codec.long_name).toBe 'MPEG-4 part 2'
      expect(ctx.video_codec.type).toBe AVMediaType.AVMEDIA_TYPE_VIDEO
      expect(ctx.video_codec.id).toBe AVCodecID.AV_CODEC_ID_MPEG4
      expect(ctx.video_codec.capabilities).toBe 8224
      expect(ctx.video_codec.supported_framerates).toEqual []
      expect(ctx.video_codec.pix_fmts).toEqual [0]
      expect(ctx.video_codec.supported_samplerates).toEqual []
      expect(ctx.video_codec.sample_fmts).toEqual []
      expect(ctx.video_codec.channel_layouts).toEqual []
      expect(ctx.video_codec.max_lowres).toBe 0
      expect(ctx.video_codec.profiles).toEqual []

    it '.audio_codec', ->
      ctx.audio_codec = avcodec.findEncoder 'flac'
      expect(ctx.audio_codec.is_encoder).toBe true
      expect(ctx.audio_codec.is_decoder).toBe false
      expect(ctx.audio_codec.name).toBe 'flac'
      expect(ctx.audio_codec.long_name).toBe 'FLAC (Free Lossless Audio Codec)'
      expect(ctx.audio_codec.type).toBe AVMediaType.AVMEDIA_TYPE_AUDIO
      expect(ctx.audio_codec.id).toBe AVCodecID.AV_CODEC_ID_FLAC
      expect(ctx.audio_codec.capabilities).toBe -2147483552
      expect(ctx.audio_codec.supported_framerates).toEqual []
      expect(ctx.audio_codec.pix_fmts).toEqual []
      expect(ctx.audio_codec.supported_samplerates).toEqual []
      expect(ctx.audio_codec.sample_fmts).toEqual [1, 2]
      expect(ctx.audio_codec.channel_layouts).toEqual []
      expect(ctx.audio_codec.max_lowres).toBe 0
      expect(ctx.audio_codec.profiles).toEqual []

    it '.subtitle_codec', ->
      ctx.subtitle_codec = avcodec.findEncoder 'ass'
      expect(ctx.subtitle_codec.is_encoder).toBe true
      expect(ctx.subtitle_codec.is_decoder).toBe false
      expect(ctx.subtitle_codec.name).toBe 'ass'
      expect(ctx.subtitle_codec.long_name).toBe 'ASS (Advanced SubStation Alpha) subtitle'
      expect(ctx.subtitle_codec.type).toBe AVMediaType.AVMEDIA_TYPE_SUBTITLE
      expect(ctx.subtitle_codec.id).toBe AVCodecID.AV_CODEC_ID_ASS
      expect(ctx.subtitle_codec.capabilities).toBe 0
      expect(ctx.subtitle_codec.supported_framerates).toEqual []
      expect(ctx.subtitle_codec.pix_fmts).toEqual []
      expect(ctx.subtitle_codec.supported_samplerates).toEqual []
      expect(ctx.subtitle_codec.sample_fmts).toEqual []
      expect(ctx.subtitle_codec.channel_layouts).toEqual []
      expect(ctx.subtitle_codec.max_lowres).toBe 0
      expect(ctx.subtitle_codec.profiles).toEqual []

    it '.data_codec', ->
      ctx.data_codec = null
      expect(ctx.data_codec).toBe null

    it '.data_codec_id', ->
      ctx.data_codec_id = AVCodecID.AV_CODEC_ID_NONE
      expect(ctx.data_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
