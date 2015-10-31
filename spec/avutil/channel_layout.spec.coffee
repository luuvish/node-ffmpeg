ffmpeg = require '../../dist/lib/ffmpeg'
avutil = ffmpeg.avutil
channel_layout = avutil.channel_layout


describe 'channel_layout', ->

  describe 'check predefined constants', ->

    it 'channel layout', ->
      expect(channel_layout.AV_CH_FRONT_LEFT).toBe 1
      expect(channel_layout.AV_CH_FRONT_RIGHT).toBe 2
      expect(channel_layout.AV_CH_FRONT_CENTER).toBe 4
      expect(channel_layout.AV_CH_BACK_LEFT).toBe 16
      expect(channel_layout.AV_CH_BACK_RIGHT).toBe 32
      expect(channel_layout.AV_CH_STEREO_LEFT).toBe 536870912
      expect(channel_layout.AV_CH_STEREO_RIGHT).toBe 1073741824
      expect(channel_layout.AV_CH_LAYOUT_NATIVE).toBe 9223372036854776000
      expect(channel_layout.AV_CH_LAYOUT_MONO).toBe 4
      expect(channel_layout.AV_CH_LAYOUT_STEREO).toBe 3
      expect(channel_layout.AV_CH_LAYOUT_SURROUND).toBe 7
      expect(channel_layout.AV_CH_LAYOUT_STEREO_DOWNMIX).toBe 1610612736

  describe 'test utility functions', ->
    getChannelLayout = channel_layout.getChannelLayout
    getChannelLayoutNbChannels = channel_layout.getChannelLayoutNbChannels
    getDefaultChannelLayout = channel_layout.getDefaultChannelLayout
    getChannelLayoutChannelIndex = channel_layout.getChannelLayoutChannelIndex
    extractChannelLayout = channel_layout.extractChannelLayout
    getChannelName = channel_layout.getChannelName
    getChannelDescription = channel_layout.getChannelDescription
    getStandardChannelLayout = channel_layout.getStandardChannelLayout

    it '.getChannelLayout()', ->
      expect(getChannelLayout 'mono+FC').toBe (channel_layout.AV_CH_LAYOUT_MONO | channel_layout.AV_CH_FRONT_CENTER)
      expect(getChannelLayout 'mono+FL').toBe (channel_layout.AV_CH_LAYOUT_MONO | channel_layout.AV_CH_FRONT_LEFT)
      expect(getChannelLayout 'mono+FR').toBe (channel_layout.AV_CH_LAYOUT_MONO | channel_layout.AV_CH_FRONT_RIGHT)
      expect(getChannelLayout 'stereo+BC').toBe (channel_layout.AV_CH_LAYOUT_STEREO | channel_layout.AV_CH_BACK_CENTER)
      expect(getChannelLayout '5.1').toBe channel_layout.AV_CH_LAYOUT_5POINT1_BACK

    it '.getChannelLayoutNbChannels()', ->
      expect(getChannelLayoutNbChannels channel_layout.AV_CH_LAYOUT_STEREO).toBe 2
      expect(getChannelLayoutNbChannels channel_layout.AV_CH_LAYOUT_5POINT1).toBe 6
      expect(getChannelLayoutNbChannels channel_layout.AV_CH_LAYOUT_7POINT1).toBe 8

    it '.getDefaultChannelLayout()', ->
      expect(getDefaultChannelLayout 0).toBe 0
      expect(getDefaultChannelLayout 1).toBe channel_layout.AV_CH_LAYOUT_MONO
      expect(getDefaultChannelLayout 2).toBe channel_layout.AV_CH_LAYOUT_STEREO
      expect(getDefaultChannelLayout 3).toBe channel_layout.AV_CH_LAYOUT_2POINT1
      expect(getDefaultChannelLayout 4).toBe channel_layout.AV_CH_LAYOUT_4POINT0
      expect(getDefaultChannelLayout 5).toBe channel_layout.AV_CH_LAYOUT_5POINT0_BACK
      expect(getDefaultChannelLayout 6).toBe channel_layout.AV_CH_LAYOUT_5POINT1_BACK
      expect(getDefaultChannelLayout 7).toBe channel_layout.AV_CH_LAYOUT_6POINT1
      expect(getDefaultChannelLayout 8).toBe channel_layout.AV_CH_LAYOUT_7POINT1
      expect(getDefaultChannelLayout 9).toBe 0

    it '.getChannelLayoutChannelIndex()', ->
      expect(getChannelLayoutChannelIndex channel_layout.AV_CH_LAYOUT_5POINT1, channel_layout.AV_CH_FRONT_LEFT).toBe 0
      expect(getChannelLayoutChannelIndex channel_layout.AV_CH_LAYOUT_5POINT1, channel_layout.AV_CH_FRONT_RIGHT).toBe 1
      expect(getChannelLayoutChannelIndex channel_layout.AV_CH_LAYOUT_5POINT1, channel_layout.AV_CH_FRONT_CENTER).toBe 2
      expect(getChannelLayoutChannelIndex channel_layout.AV_CH_LAYOUT_5POINT1, channel_layout.AV_CH_STEREO_LEFT).toBe -22

    it '.extractChannelLayout()', ->
      expect(extractChannelLayout channel_layout.AV_CH_LAYOUT_5POINT1, 0).toBe channel_layout.AV_CH_FRONT_LEFT
      expect(extractChannelLayout channel_layout.AV_CH_LAYOUT_5POINT1, 1).toBe channel_layout.AV_CH_FRONT_RIGHT
      expect(extractChannelLayout channel_layout.AV_CH_LAYOUT_5POINT1, 2).toBe channel_layout.AV_CH_FRONT_CENTER
      expect(extractChannelLayout channel_layout.AV_CH_LAYOUT_5POINT1, 3).toBe channel_layout.AV_CH_LOW_FREQUENCY
      expect(extractChannelLayout channel_layout.AV_CH_LAYOUT_5POINT1, 4).toBe channel_layout.AV_CH_SIDE_LEFT
      expect(extractChannelLayout channel_layout.AV_CH_LAYOUT_5POINT1, 5).toBe channel_layout.AV_CH_SIDE_RIGHT

    it '.getChannelName()', ->
      expect(getChannelName 0).toBe ''
      expect(getChannelName 1).toBe 'FL'
      expect(getChannelName 2).toBe 'FR'
      expect(getChannelName 4).toBe 'FC'
      expect(getChannelName 8).toBe 'LFE'

    it '.getChannelDescription()', ->
      expect(getChannelDescription 0).toBe ''
      expect(getChannelDescription 1).toBe 'front left'
      expect(getChannelDescription 2).toBe 'front right'
      expect(getChannelDescription 4).toBe 'front center'
      expect(getChannelDescription 8).toBe 'low frequency'

    it '.getStandardChannelLayout()', ->
      expect(getStandardChannelLayout 0).toEqual [channel_layout.AV_CH_LAYOUT_MONO, 'mono']
      expect(getStandardChannelLayout 1).toEqual [channel_layout.AV_CH_LAYOUT_STEREO, 'stereo']
      expect(getStandardChannelLayout 2).toEqual [channel_layout.AV_CH_LAYOUT_2POINT1, '2.1']
      expect(getStandardChannelLayout 3).toEqual [channel_layout.AV_CH_LAYOUT_SURROUND, '3.0']
      expect(getStandardChannelLayout 4).toEqual [channel_layout.AV_CH_LAYOUT_2_1, '3.0(back)']
      expect(getStandardChannelLayout 5).toEqual [channel_layout.AV_CH_LAYOUT_4POINT0, '4.0']
      expect(getStandardChannelLayout 6).toEqual [channel_layout.AV_CH_LAYOUT_QUAD, 'quad']
      expect(getStandardChannelLayout 7).toEqual [channel_layout.AV_CH_LAYOUT_2_2, 'quad(side)']
      expect(getStandardChannelLayout 8).toEqual [channel_layout.AV_CH_LAYOUT_3POINT1, '3.1']
      expect(getStandardChannelLayout 9).toEqual [channel_layout.AV_CH_LAYOUT_5POINT0_BACK, '5.0']
      expect(getStandardChannelLayout 10).toEqual [channel_layout.AV_CH_LAYOUT_5POINT0, '5.0(side)']
      expect(getStandardChannelLayout 11).toEqual [channel_layout.AV_CH_LAYOUT_4POINT1, '4.1']
      expect(getStandardChannelLayout 12).toEqual [channel_layout.AV_CH_LAYOUT_5POINT1_BACK, '5.1']
      expect(getStandardChannelLayout 13).toEqual [channel_layout.AV_CH_LAYOUT_5POINT1, '5.1(side)']
      expect(getStandardChannelLayout 14).toEqual [channel_layout.AV_CH_LAYOUT_6POINT0, '6.0']
      expect(getStandardChannelLayout 15).toEqual [channel_layout.AV_CH_LAYOUT_6POINT0_FRONT, '6.0(front)']
      expect(getStandardChannelLayout 16).toEqual [channel_layout.AV_CH_LAYOUT_HEXAGONAL, 'hexagonal']
      expect(getStandardChannelLayout 17).toEqual [channel_layout.AV_CH_LAYOUT_6POINT1, '6.1']
      expect(getStandardChannelLayout 18).toEqual [channel_layout.AV_CH_LAYOUT_6POINT1_BACK, '6.1']
      expect(getStandardChannelLayout 19).toEqual [channel_layout.AV_CH_LAYOUT_6POINT1_FRONT, '6.1(front)']
      expect(getStandardChannelLayout 20).toEqual [channel_layout.AV_CH_LAYOUT_7POINT0, '7.0']
      expect(getStandardChannelLayout 21).toEqual [channel_layout.AV_CH_LAYOUT_7POINT0_FRONT, '7.0(front)']
      expect(getStandardChannelLayout 22).toEqual [channel_layout.AV_CH_LAYOUT_7POINT1, '7.1']
      expect(getStandardChannelLayout 23).toEqual [channel_layout.AV_CH_LAYOUT_7POINT1_WIDE_BACK, '7.1(wide)']
      expect(getStandardChannelLayout 24).toEqual [channel_layout.AV_CH_LAYOUT_7POINT1_WIDE, '7.1(wide-side)']
      expect(getStandardChannelLayout 25).toEqual [channel_layout.AV_CH_LAYOUT_OCTAGONAL, 'octagonal']
      expect(getStandardChannelLayout 26).toEqual [channel_layout.AV_CH_LAYOUT_STEREO_DOWNMIX, 'downmix']
