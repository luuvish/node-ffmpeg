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

### AVInputFormat in libavformat/avformat.h

```javascript
var mp4 = FFmpeg.AVInputFormat.findInputFormat('mp4');
mp4.name == 'mov,mp4,m4a,3gp,3g2,mj2';
mp4.long_name == 'QuickTime / MOV';

var h264 = FFmpeg.AVInputFormat.findInputFormat('h264');
h264.name == 'h264';
h264.long_name == 'raw H.264 video';
```

### AVOutputFormat in libavformat/avformat.h

```javascript
var webm = FFmpeg.AVOutputFormat.guessFormat('', '', 'webm');
webm.name == 'webm';
webm.long_name == 'WebM';
webm.mime_type == 'video/webm';
webm.extensions == 'webm';

var vc1 = FFmpeg.AVOutputFormat.guessFormat('', '', 'vc1');
vc1.name == 'vc1';
vc1.long_name == 'raw VC-1 video';
vc1.mime_type == 'video/vc1';
vc1.extensions == 'vc1';

71 == FFmpeg.AVOutputFormat.guessCodec(vc1, '', '', '', 0);
FFmpeg.AVOutputFormat.queryCodec(vc1, 71, 0);
```

### AVFormatContext in libavformat/avformat.h

```javascript
var fmtctx = new FFmpeg.AVFormatContext();
fmtctx.openInput('./test/test.mp4', {});
fmtctx.dumpFormat(0, './test/test.mp4', 0);

for (var s in fmtctx.streams) {
  console.log(s.codec);
}
for (var p in fmtctx.programs) {
  console.log(p.id);
}
for (var c in fmtctx.chapters) {
  console.log(c.id);
}

fmtctx.closeInput();
```

### AVStream in libavformat/avformat.h

```javascript
var fmtctx = new FFmpeg.AVFormatContext();
fmtctx.openInput('./test/test.mp4', {});
var stream = fmtctx.streams[0];

stream.index == 0;
stream.id;
stream.codec;
stream.pts == {val: 0, num: 0, den: 0};
stream.time_base == {num: 1, den: 1000};
stream.start_time == FFmpeg.AV_NOPTS_VALUE;
stream.duration == FFmpeg.AV_NOPTS_VALUE;
stream.discard == 0;
stream.sample_aspect_ratio == {num: 1, den: 1};
```

### AVCodecContext in libavcodec/avcodec.h

```javascript
var fmtctx = new FFmpeg.AVFormatContext();
fmtctx.openInput('./test/test.mp4', {});

var video_index = fmtctx.findBestStream(FFmpeg.AVMEDIA_TYPE_VIDEO, -1, -1, 0);
var audio_index = fmtctx.findBestStream(FFmpeg.AVMEDIA_TYPE_AUDIO, -1, video_index, 0);
var subtt_index = fmtctx.findBestStream(FFmpeg.AVMEDIA_TYPE_SUBTITLE, -1, audio_index, 0);

var video_ctx = fmtctx.streams[video_index].codec;
var video_codec = FFmpeg.AVCodec.findDecoder(video_ctx.codec_id);

video_ctx.lowres = Math.min(video_ctx.lowres, video_codec.max_lowres);
video_ctx.workaround_bugs == 1;
video_ctx.error_concealment == 3;

video_ctx.open(video_codec, {
  lowres: video_ctx.lowres,
  threads: 'auto',
  refcounted_frames: 1
});

video_ctx.is_open == true;
video_ctx.codec_type == FFmpeg.AVMEDIA_TYPE_VIDEO;
video_ctx.codec_id == 28;
video_ctx.time_base == {num: 0, den: 2};
video_ctx.width == 1920;
video_ctx.height == 1080;

var audio_ctx = fmtctx.streams[audio_index].codec;
audio_ctx.open();
audio_ctx.codec_type == FFmpeg.AVMEDIA_TYPE_AUDIO;
audio_ctx.codec_id == 86020;
audio_ctx.time_base == {num: 1, den:48000};
audio_ctx.sample_rate == 48000;
audio_ctx.sample_fmt == 8;
audio_ctx.channels == 2;
audio_ctx.channel_layout == 0;
audio_ctx.close();

var subtt_ctx = fmtctx.streams[subtt_index].codec;
subtt_ctx.open();
subtt_ctx.codec_type == FFmpeg.AVMEDIA_TYPE_SUBTITLE;
subtt_ctx.codec_id == 94208;
subtt_ctx.time_base == {num: 0, den: 1};
subtt_ctx.width == 1920;
subtt_ctx.height == 1080;
subtt_ctx.close();
```
