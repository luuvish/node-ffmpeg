node-ffmpeg
===========

FFmpeg node modules


## install (Mac OS X)

```bash
git clone https://github.com/luuvish/node-ffmpeg
npm install
```


## usage

```javascript
var FFmpeg = require('./lib/ffmpeg');
```

### AVCodec in libavcodec/avcodec.h

```javascript
var codec = FFmpeg.AVCodec.findDecoder('h264');

codec.is_encoder == false;
codec.is_decoder == true;
codec.name == 'h264';
codec.long_name;
codec.type == 0; // AVMEDIA_TYPE_VIDEO
codec.id == 28;
codec.max_lowres;

codec.name == FFmpeg.AVCodec.getName(codec.id);
codec.type == FFmpeg.AVCodec.getType(codec.id);
```

### AVFormatContext in libavformat/avformat.h

```javascript
var fmtctx = new FFmpeg.AVFormatContext();

fmtctx.openInput('./test/test.mp4', None, {
  genpts: true,
  seek_by_bytes: -1,
  title: 'test.mp4',
  start_time: ffmpeg.AV_NOPTS_VALUE,
  infinite_buffer: -1,
  show_status: true
});

for (var s in fmtctx.streams) {
  console.log(s.codec);
}
```

### AVCodecContext in libavcodec/avcodec.h

```javascript
var fmtctx = new FFmpeg.AVFormatContext();

fmtctx.openInput('examples/sherlock-01.mkv');

var ctx = fmtctx.streams[0].codec;
var codec = FFmpeg.AVCodec.findDecoder(ctx.codec_id);

ctx.lowres = Math.min(ctx.lowres, codec.max_lowres);
ctx.workaround_bugs == 1;
ctx.error_concealment == 3;

ctx.open(codec, {
  lowres: ctx.lowres,
  threads: 'auto',
  refcounted_frames: 1
});
```
