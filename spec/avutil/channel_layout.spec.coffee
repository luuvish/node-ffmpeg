ffmpeg = require '../../lib/ffmpeg'

describe 'ffmpeg.avutil.channel_layout', ->
  ch = ffmpeg.avutil.channel_layout

  it 'constants', ->
    expect(ch.AV_CH_FRONT_LEFT).toBe 1
    expect(ch.AV_CH_FRONT_RIGHT).toBe 2
    expect(ch.AV_CH_FRONT_CENTER).toBe 4
    expect(ch.AV_CH_BACK_LEFT).toBe 16
    expect(ch.AV_CH_BACK_RIGHT).toBe 32
    expect(ch.AV_CH_STEREO_LEFT).toBe 536870912
    expect(ch.AV_CH_STEREO_RIGHT).toBe 1073741824
    expect(ch.AV_CH_LAYOUT_NATIVE).toBe 9223372036854776000
    expect(ch.AV_CH_LAYOUT_MONO).toBe 4
    expect(ch.AV_CH_LAYOUT_STEREO).toBe 3
    expect(ch.AV_CH_LAYOUT_SURROUND).toBe 7
    expect(ch.AV_CH_LAYOUT_STEREO_DOWNMIX).toBe 1610612736

  it 'getChannelLayout()', ->
    getChannelLayout = ch.getChannelLayout
    expect(getChannelLayout 'mono+FC').toBe (ch.AV_CH_LAYOUT_MONO | ch.AV_CH_FRONT_CENTER)
    expect(getChannelLayout 'mono+FL').toBe (ch.AV_CH_LAYOUT_MONO | ch.AV_CH_FRONT_LEFT)
    expect(getChannelLayout 'mono+FR').toBe (ch.AV_CH_LAYOUT_MONO | ch.AV_CH_FRONT_RIGHT)
    expect(getChannelLayout 'stereo+BC').toBe (ch.AV_CH_LAYOUT_STEREO | ch.AV_CH_BACK_CENTER)
    expect(getChannelLayout '5.1').toBe ch.AV_CH_LAYOUT_5POINT1_BACK

  it 'getChannelLayoutNbChannels()', ->
    getChannelLayoutNbChannels = ch.getChannelLayoutNbChannels
    expect(getChannelLayoutNbChannels ch.AV_CH_LAYOUT_STEREO).toBe 2
    expect(getChannelLayoutNbChannels ch.AV_CH_LAYOUT_5POINT1).toBe 6
    expect(getChannelLayoutNbChannels ch.AV_CH_LAYOUT_7POINT1).toBe 8

  it 'getDefaultChannelLayout()', ->
    getDefaultChannelLayout = ch.getDefaultChannelLayout
    expect(getDefaultChannelLayout 0).toBe 0
    expect(getDefaultChannelLayout 1).toBe ch.AV_CH_LAYOUT_MONO
    expect(getDefaultChannelLayout 2).toBe ch.AV_CH_LAYOUT_STEREO
    expect(getDefaultChannelLayout 3).toBe ch.AV_CH_LAYOUT_2POINT1
    expect(getDefaultChannelLayout 4).toBe ch.AV_CH_LAYOUT_4POINT0
    expect(getDefaultChannelLayout 5).toBe ch.AV_CH_LAYOUT_5POINT0_BACK
    expect(getDefaultChannelLayout 6).toBe ch.AV_CH_LAYOUT_5POINT1_BACK
    expect(getDefaultChannelLayout 7).toBe ch.AV_CH_LAYOUT_6POINT1
    expect(getDefaultChannelLayout 8).toBe ch.AV_CH_LAYOUT_7POINT1
    expect(getDefaultChannelLayout 9).toBe 0

  it 'getChannelLayoutChannelIndex()', ->
    getChannelLayoutChannelIndex = ch.getChannelLayoutChannelIndex
    expect(getChannelLayoutChannelIndex ch.AV_CH_LAYOUT_5POINT1, ch.AV_CH_FRONT_LEFT).toBe 0
    expect(getChannelLayoutChannelIndex ch.AV_CH_LAYOUT_5POINT1, ch.AV_CH_FRONT_RIGHT).toBe 1
    expect(getChannelLayoutChannelIndex ch.AV_CH_LAYOUT_5POINT1, ch.AV_CH_FRONT_CENTER).toBe 2
    expect(getChannelLayoutChannelIndex ch.AV_CH_LAYOUT_5POINT1, ch.AV_CH_STEREO_LEFT).toBe -22

  it 'extractChannelLayout()', ->
    extractChannelLayout = ch.extractChannelLayout
    expect(extractChannelLayout ch.AV_CH_LAYOUT_5POINT1, 0).toBe ch.AV_CH_FRONT_LEFT
    expect(extractChannelLayout ch.AV_CH_LAYOUT_5POINT1, 1).toBe ch.AV_CH_FRONT_RIGHT
    expect(extractChannelLayout ch.AV_CH_LAYOUT_5POINT1, 2).toBe ch.AV_CH_FRONT_CENTER
    expect(extractChannelLayout ch.AV_CH_LAYOUT_5POINT1, 3).toBe ch.AV_CH_LOW_FREQUENCY
    expect(extractChannelLayout ch.AV_CH_LAYOUT_5POINT1, 4).toBe ch.AV_CH_SIDE_LEFT
    expect(extractChannelLayout ch.AV_CH_LAYOUT_5POINT1, 5).toBe ch.AV_CH_SIDE_RIGHT

  it 'getChannelName()', ->
    getChannelName = ch.getChannelName
    expect(getChannelName 0).toBe ''
    expect(getChannelName 1).toBe 'FL'
    expect(getChannelName 2).toBe 'FR'
    expect(getChannelName 4).toBe 'FC'
    expect(getChannelName 8).toBe 'LFE'

  it 'getChannelDescription()', ->
    getChannelDescription = ch.getChannelDescription
    expect(getChannelDescription 0).toBe ''
    expect(getChannelDescription 1).toBe 'front left'
    expect(getChannelDescription 2).toBe 'front right'
    expect(getChannelDescription 4).toBe 'front center'
    expect(getChannelDescription 8).toBe 'low frequency'

  it 'getStandardChannelLayout()', ->
    getStandardChannelLayout = ch.getStandardChannelLayout
    expect(getStandardChannelLayout 0).toEqual [ch.AV_CH_LAYOUT_MONO, 'mono']
    expect(getStandardChannelLayout 1).toEqual [ch.AV_CH_LAYOUT_STEREO, 'stereo']
    expect(getStandardChannelLayout 2).toEqual [ch.AV_CH_LAYOUT_2POINT1, '2.1']
    expect(getStandardChannelLayout 3).toEqual [ch.AV_CH_LAYOUT_SURROUND, '3.0']
    expect(getStandardChannelLayout 4).toEqual [ch.AV_CH_LAYOUT_2_1, '3.0(back)']
    expect(getStandardChannelLayout 5).toEqual [ch.AV_CH_LAYOUT_4POINT0, '4.0']
    expect(getStandardChannelLayout 6).toEqual [ch.AV_CH_LAYOUT_QUAD, 'quad']
    expect(getStandardChannelLayout 7).toEqual [ch.AV_CH_LAYOUT_2_2, 'quad(side)']
    expect(getStandardChannelLayout 8).toEqual [ch.AV_CH_LAYOUT_3POINT1, '3.1']
    expect(getStandardChannelLayout 9).toEqual [ch.AV_CH_LAYOUT_5POINT0_BACK, '5.0']
    expect(getStandardChannelLayout 10).toEqual [ch.AV_CH_LAYOUT_5POINT0, '5.0(side)']
    expect(getStandardChannelLayout 11).toEqual [ch.AV_CH_LAYOUT_4POINT1, '4.1']
    expect(getStandardChannelLayout 12).toEqual [ch.AV_CH_LAYOUT_5POINT1_BACK, '5.1']
    expect(getStandardChannelLayout 13).toEqual [ch.AV_CH_LAYOUT_5POINT1, '5.1(side)']
    expect(getStandardChannelLayout 14).toEqual [ch.AV_CH_LAYOUT_6POINT0, '6.0']
    expect(getStandardChannelLayout 15).toEqual [ch.AV_CH_LAYOUT_6POINT0_FRONT, '6.0(front)']
    expect(getStandardChannelLayout 16).toEqual [ch.AV_CH_LAYOUT_HEXAGONAL, 'hexagonal']
    expect(getStandardChannelLayout 17).toEqual [ch.AV_CH_LAYOUT_6POINT1, '6.1']
    expect(getStandardChannelLayout 18).toEqual [ch.AV_CH_LAYOUT_6POINT1_BACK, '6.1']
    expect(getStandardChannelLayout 19).toEqual [ch.AV_CH_LAYOUT_6POINT1_FRONT, '6.1(front)']
    expect(getStandardChannelLayout 20).toEqual [ch.AV_CH_LAYOUT_7POINT0, '7.0']
    expect(getStandardChannelLayout 21).toEqual [ch.AV_CH_LAYOUT_7POINT0_FRONT, '7.0(front)']
    expect(getStandardChannelLayout 22).toEqual [ch.AV_CH_LAYOUT_7POINT1, '7.1']
    expect(getStandardChannelLayout 23).toEqual [ch.AV_CH_LAYOUT_7POINT1_WIDE_BACK, '7.1(wide)']
    expect(getStandardChannelLayout 24).toEqual [ch.AV_CH_LAYOUT_7POINT1_WIDE, '7.1(wide-side)']
    expect(getStandardChannelLayout 25).toEqual [ch.AV_CH_LAYOUT_OCTAGONAL, 'octagonal']
    expect(getStandardChannelLayout 26).toEqual [ch.AV_CH_LAYOUT_STEREO_DOWNMIX, 'downmix']
