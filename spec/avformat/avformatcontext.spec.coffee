ffmpeg = require '../../lib/ffmpeg'

describe 'ffmpeg.avformat', ->
  avformat = ffmpeg.avformat
  avcodec = ffmpeg.avcodec
  avutil = ffmpeg.avutil
  AVCodecID = ffmpeg.avcodec.AVCodecID
  AVMediaType = ffmpeg.avutil.AVMediaType

  it 'input AVFormatContext (mkv)', ->
    ctx = new avformat.AVFormatContext
    ctx.openInput './examples/ticket-a_aac.mkv', {}

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
    ctx = null

  it 'input AVFormatContext (mp4)', ->
    ctx = new avformat.AVFormatContext
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
    ctx = null

  it 'output AVFormatContext', ->
    ctx = new avformat.AVFormatContext
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
    ctx = null

  xit 'AVFormatContext.findStreamInfo()', ->
  xit 'AVFormatContext.findProgramFromStream()', ->
  xit 'AVFormatContext.findBestStream()', ->
  xit 'AVFormatContext.readFrame()', ->
  xit 'AVFormatContext.seekFrame()', ->
  xit 'AVFormatContext.seekFile()', ->
  xit 'AVFormatContext.flush()', ->
  xit 'AVFormatContext.readPlay()', ->
  xit 'AVFormatContext.readPause()', ->
  xit 'AVFormatContext.writeHeader()', ->
  xit 'AVFormatContext.writeFrame()', ->
  xit 'AVFormatContext.writeInterleavedFrame()', ->
  xit 'AVFormatContext.writeUncodedFrame()', ->
  xit 'AVFormatContext.writeInterleavedUncodedFrame()', ->
  xit 'AVFormatContext.writeUncodedFrameQuery()', ->
  xit 'AVFormatContext.writeTrailer()', ->
  xit 'AVFormatContext.getOutputTimestamp()', ->
  xit 'AVFormatContext.findDefaultStreamIndex()', ->
  xit 'AVFormatContext.dumpFormat()', ->
  xit 'AVFormatContext.guessSampleAspectRatio()', ->
  xit 'AVFormatContext.guessFrameRate()', ->
  xit 'AVFormatContext.matchStreamSpecifier()', ->
  xit 'AVFormatContext.queueAttachedPictures()', ->
  xit 'AVFormatContext.injectGlobalSideData()', ->

  it 'AVFormatContext getter/setter test', ->
    ctx = new avformat.AVFormatContext

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

    ctx.filename = 'test.h264'
    expect(ctx.filename).toBe 'test.h264'

    ctx.flags = 71
    expect(ctx.flags).toBe 71

    ctx.video_codec_id = AVCodecID.AV_CODEC_ID_H264
    expect(ctx.video_codec_id).toBe AVCodecID.AV_CODEC_ID_H264

    ctx.audio_codec_id = AVCodecID.AV_CODEC_ID_AC3
    expect(ctx.audio_codec_id).toBe AVCodecID.AV_CODEC_ID_AC3

    ctx.subtitle_codec_id = AVCodecID.AV_CODEC_ID_ASS
    expect(ctx.subtitle_codec_id).toBe AVCodecID.AV_CODEC_ID_ASS

    ctx.max_index_size = 1000
    expect(ctx.max_index_size).toBe 1000

    ctx.start_time_realtime = 2000
    expect(ctx.start_time_realtime).toBe 2000

    ctx.fps_probe_size = 12
    expect(ctx.fps_probe_size).toBe 12

    ctx.error_recognition = 11
    expect(ctx.error_recognition).toBe 11

    ctx.max_interleave_delta = 400000
    expect(ctx.max_interleave_delta).toBe 400000

    ctx.strict_std_compliance = 2
    expect(ctx.strict_std_compliance).toBe 2

    ctx.event_flags = 5
    expect(ctx.event_flags).toBe 5

    ctx.max_ts_probe = 60
    expect(ctx.max_ts_probe).toBe 60

    ctx.avoid_negative_ts = 8
    expect(ctx.avoid_negative_ts).toBe 8

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

    ctx.data_codec = null
    expect(ctx.data_codec).toBe null

    ctx.data_codec_id = AVCodecID.AV_CODEC_ID_NONE
    expect(ctx.data_codec_id).toBe AVCodecID.AV_CODEC_ID_NONE
