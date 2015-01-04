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
        'src/addons/main.cc',
        'src/addons/avformat.cc',
        'src/addons/avcodec.cc',
        'src/addons/avutil.cc'
      ],
      'include_dirs': [
        '<!(node -e \"require(\'nan\')\")',
        'tmp/ffmpeg-lib/include'
      ],
      'link_settings': {
        'libraries': [
          '-lavcodec',
          '-lavdevice',
          '-lavfilter',
          '-lavformat',
          '-lavresample',
          '-lavutil',
          '-lpostproc',
          '-lswscale',
          '-lswresample'
        ],
        'library_dirs': [
          '../tmp/ffmpeg-lib/lib'
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
