{
  'conditions': [
    [ 'OS=="win"', {
      'variables': {}
    }, { # 'OS!="win"'
      'variables': {}
    }]
  ],
  'targets': [
    {
      'target_name': 'ffmpeg',
      'sources': [
        'src/addons/ffmpeg.cc',
        'src/addons/avformat/avformat.cc',
        'src/addons/avformat/avformatcontext.cc',
        'src/addons/avformat/avinputformat.cc',
        'src/addons/avformat/avoutputformat.cc',
        'src/addons/avformat/avstream.cc',
        'src/addons/avformat/avprogram.cc',
        'src/addons/avformat/avchapter.cc',
        'src/addons/avcodec/avcodec.cc',
        'src/addons/avcodec/avcodeccontext.cc',
        'src/addons/avcodec/avpacket.cc',
        'src/addons/avcodec/avpicture.cc',
        'src/addons/avcodec/avsubtitle.cc',
        'src/addons/avcodec/avsubtitlerect.cc',
        'src/addons/avutil/avutil.cc',
        'src/addons/avutil/channel_layout.cc',
        'src/addons/avutil/pixfmt.cc',
        'src/addons/avutil/samplefmt.cc',
        'src/addons/avutil/avframe.cc'
      ],
      'include_dirs': [
        '<!(node -e \"require(\'nan\')\")',
        'dist/ffmpeg-2.6/include',
        'src/addons'
      ],
      'link_settings': {
        'libraries': [
          '../dist/ffmpeg-2.6/lib/libavcodec.a',
          '../dist/ffmpeg-2.6/lib/libavdevice.a',
          '../dist/ffmpeg-2.6/lib/libavfilter.a',
          '../dist/ffmpeg-2.6/lib/libavformat.a',
          '../dist/ffmpeg-2.6/lib/libavutil.a',
          '../dist/ffmpeg-2.6/lib/libswscale.a',
          '../dist/ffmpeg-2.6/lib/libswresample.a',
          #'-lavcodec',
          #'-lavdevice',
          #'-lavfilter',
          #'-lavformat',
          #'-lavresample',
          #'-lavutil',
          #'-lpostproc',
          #'-lswscale',
          #'-lswresample'
        ],
        'library_dirs': [
          '../dist/ffmpeg-2.6/lib'
        ]
      },
      'conditions': [
        [ 'OS=="win"', {
          'sources': [],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ExceptionHandling': 1, # /EHsc
              'WarnAsError': 'true'
            }
          },
          'msvs_disabled_warnings': [
            4018,  # signed/unsigned mismatch
            4244,  # conversion from 'type1' to 'type2', possible loss of data
            4267,  # conversion from 'size_t' to 'type', possible loss of data
            4530,  # C++ exception handler used, but unwind semantics are not enabled
            4506,  # no definition for inline function
            4996,  # function was declared deprecated
          ],
          'defines': [
            '_WIN32_WINNT=0x0600'
          ]
        }], # OS=="win"
        [ 'OS=="mac"', {
          'sources': [],
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS' : [
              '-std=c++11',
              '-stdlib=libc++'
            ],
            'OTHER_LDFLAGS': [
              '-stdlib=libc++',
              '-framework CoreFoundation',
              '-framework VideoDecodeAcceleration',
              '-framework QuartzCore',
              '-Wl,-framework,Cocoa'
            ],
            'MACOSX_DEPLOYMENT_TARGET': '10.7'
          }
        }], # OS=="mac"
        [ 'OS=="linux"', {
          'sources': []
        }]
      ]
    }
  ]
}
