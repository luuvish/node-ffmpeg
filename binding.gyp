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
        'src/ffmpeg/main.cc',
        'src/ffmpeg/avformat.cc',
        'src/ffmpeg/avcodec.cc',
        'src/ffmpeg/avutil.cc'
      ],
      'include_dirs': [
        '<!(node -e \"require(\'nan\')\")',
        'tmp/ffmpeg/include'
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
          '../tmp/ffmpeg/lib'
        ]
      },
      'conditions': [
        [ 'OS=="mac"', {
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
        }]
      ]
    }
  ]
}
