ffmpeg = require '../../lib/ffmpeg'

describe 'AVPixelFormat constants', ->
  AVPixelFormat = ffmpeg.avutil.AVPixelFormat

  it 'YUV or RGB format', ->
    expect(AVPixelFormat.AV_PIX_FMT_NONE).toBe -1
    expect(AVPixelFormat.AV_PIX_FMT_YUV420P).toBe 0
    expect(AVPixelFormat.AV_PIX_FMT_YUYV422).toBe 1
    expect(AVPixelFormat.AV_PIX_FMT_RGB24).toBe 2
    expect(AVPixelFormat.AV_PIX_FMT_BGR24).toBe 3
    expect(AVPixelFormat.AV_PIX_FMT_YUV422P).toBe 4
    expect(AVPixelFormat.AV_PIX_FMT_YUV444P).toBe 5

  it 'BAYER format', ->
    expect(AVPixelFormat.AV_PIX_FMT_BAYER_BGGR16LE).toBe 325
    expect(AVPixelFormat.AV_PIX_FMT_BAYER_BGGR16BE).toBe 326
    expect(AVPixelFormat.AV_PIX_FMT_BAYER_RGGB16LE).toBe 327
    expect(AVPixelFormat.AV_PIX_FMT_BAYER_RGGB16BE).toBe 328
    expect(AVPixelFormat.AV_PIX_FMT_BAYER_GBRG16LE).toBe 329
    expect(AVPixelFormat.AV_PIX_FMT_BAYER_GBRG16BE).toBe 330
    expect(AVPixelFormat.AV_PIX_FMT_BAYER_GRBG16LE).toBe 331
    expect(AVPixelFormat.AV_PIX_FMT_BAYER_GRBG16BE).toBe 332

describe 'AVColorPrimaries constants', ->
  AVColorPrimaries = ffmpeg.avutil.AVColorPrimaries

  it 'constants', ->
    expect(AVColorPrimaries.AVCOL_PRI_RESERVED0).toBe 0
    expect(AVColorPrimaries.AVCOL_PRI_BT709).toBe 1
    expect(AVColorPrimaries.AVCOL_PRI_UNSPECIFIED).toBe 2
    expect(AVColorPrimaries.AVCOL_PRI_RESERVED).toBe 3
    expect(AVColorPrimaries.AVCOL_PRI_BT470M).toBe 4
    expect(AVColorPrimaries.AVCOL_PRI_BT470BG).toBe 5
    expect(AVColorPrimaries.AVCOL_PRI_SMPTE170M).toBe 6
    expect(AVColorPrimaries.AVCOL_PRI_SMPTE240M).toBe 7
    expect(AVColorPrimaries.AVCOL_PRI_FILM).toBe 8
    expect(AVColorPrimaries.AVCOL_PRI_BT2020).toBe 9

describe 'AVColorTransferCharacteristic constants', ->
  AVColorTransferCharacteristic = ffmpeg.avutil.AVColorTransferCharacteristic

  it 'constants', ->
    expect(AVColorTransferCharacteristic.AVCOL_TRC_RESERVED0).toBe 0
    expect(AVColorTransferCharacteristic.AVCOL_TRC_BT709).toBe 1
    expect(AVColorTransferCharacteristic.AVCOL_TRC_UNSPECIFIED).toBe 2
    expect(AVColorTransferCharacteristic.AVCOL_TRC_RESERVED).toBe 3
    expect(AVColorTransferCharacteristic.AVCOL_TRC_GAMMA22).toBe 4
    expect(AVColorTransferCharacteristic.AVCOL_TRC_GAMMA28).toBe 5
    expect(AVColorTransferCharacteristic.AVCOL_TRC_SMPTE170M).toBe 6
    expect(AVColorTransferCharacteristic.AVCOL_TRC_SMPTE240M).toBe 7
    expect(AVColorTransferCharacteristic.AVCOL_TRC_LINEAR).toBe 8
    expect(AVColorTransferCharacteristic.AVCOL_TRC_LOG).toBe 9
    expect(AVColorTransferCharacteristic.AVCOL_TRC_LOG_SQRT).toBe 10
    expect(AVColorTransferCharacteristic.AVCOL_TRC_IEC61966_2_4).toBe 11
    expect(AVColorTransferCharacteristic.AVCOL_TRC_BT1361_ECG).toBe 12
    expect(AVColorTransferCharacteristic.AVCOL_TRC_IEC61966_2_1).toBe 13
    expect(AVColorTransferCharacteristic.AVCOL_TRC_BT2020_10).toBe 14
    expect(AVColorTransferCharacteristic.AVCOL_TRC_BT2020_12).toBe 15

describe 'AVColorSpace constants', ->
  AVColorSpace = ffmpeg.avutil.AVColorSpace

  it 'constants', ->
    expect(AVColorSpace.AVCOL_SPC_RGB).toBe 0
    expect(AVColorSpace.AVCOL_SPC_BT709).toBe 1
    expect(AVColorSpace.AVCOL_SPC_UNSPECIFIED).toBe 2
    expect(AVColorSpace.AVCOL_SPC_RESERVED).toBe 3
    expect(AVColorSpace.AVCOL_SPC_FCC).toBe 4
    expect(AVColorSpace.AVCOL_SPC_BT470BG).toBe 5
    expect(AVColorSpace.AVCOL_SPC_SMPTE170M).toBe 6
    expect(AVColorSpace.AVCOL_SPC_SMPTE240M).toBe 7
    expect(AVColorSpace.AVCOL_SPC_YCOCG).toBe 8
    expect(AVColorSpace.AVCOL_SPC_YCGCO).toBe 8
    expect(AVColorSpace.AVCOL_SPC_BT2020_NCL).toBe 9
    expect(AVColorSpace.AVCOL_SPC_BT2020_CL).toBe 10

  it 'getColorspaceName', ->
    getColorspaceName = AVColorSpace.getColorspaceName
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_RGB).toBe 'GBR'
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_BT709).toBe 'bt709'
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_UNSPECIFIED).toBe ''
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_RESERVED).toBe ''
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_BT470BG).toBe 'bt470bg'
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_SMPTE170M).toBe 'smpte170m'
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_SMPTE240M).toBe 'smpte240m'
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_YCGCO).toBe 'YCgCo'
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_YCOCG).toBe 'YCgCo'
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_BT2020_NCL).toBe ''
    expect(getColorspaceName AVColorSpace.AVCOL_SPC_BT2020_CL).toBe ''

describe 'AVColorRange constants', ->
  AVColorRange = ffmpeg.avutil.AVColorRange

  it 'constants', ->
    expect(AVColorRange.AVCOL_RANGE_UNSPECIFIED).toBe 0
    expect(AVColorRange.AVCOL_RANGE_MPEG).toBe 1
    expect(AVColorRange.AVCOL_RANGE_JPEG).toBe 2

describe 'AVChromaLocation constants', ->
  AVChromaLocation = ffmpeg.avutil.AVChromaLocation

  it 'constants', ->
    expect(AVChromaLocation.AVCHROMA_LOC_UNSPECIFIED).toBe 0
    expect(AVChromaLocation.AVCHROMA_LOC_LEFT).toBe 1
    expect(AVChromaLocation.AVCHROMA_LOC_CENTER).toBe 2
    expect(AVChromaLocation.AVCHROMA_LOC_TOPLEFT).toBe 3
    expect(AVChromaLocation.AVCHROMA_LOC_TOP).toBe 4
    expect(AVChromaLocation.AVCHROMA_LOC_BOTTOMLEFT).toBe 5
    expect(AVChromaLocation.AVCHROMA_LOC_BOTTOM).toBe 6
