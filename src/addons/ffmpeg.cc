#include <nan.h>

#include "avcodec/avpicture.h"
#include "avcodec/avsubtitle.h"
#include "avcodec/avsubtitlerect.h"
//#include "avformat/avformat.h"
//#include "avcodec/avcodec.h"
//#include "avutil/avutil.h"
#include "avutil/pixfmt.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  //NanScope();

  //av_register_all();
  avcodec_register_all();
  //avformat_network_init();

  ffmpeg::avcodec::AVSubtitle::Init(exports);
  ffmpeg::avcodec::AVSubtitleRect::Init(exports);
  ffmpeg::avcodec::AVPicture::Init(exports);

  ffmpeg::avutil::AVPixelFormat::Init(exports);
  ffmpeg::avutil::AVColorPrimaries::Init(exports);
  ffmpeg::avutil::AVColorTransferCharacteristic::Init(exports);
  ffmpeg::avutil::AVColorSpace::Init(exports);
  ffmpeg::avutil::AVColorRange::Init(exports);
  ffmpeg::avutil::AVChromaLocation::Init(exports);
  //FFmpeg::AVFormat::Initialize(exports);
  //FFmpeg::AVCodec::Initialize(exports);
  //FFmpeg::AVUtil::Initialize(exports);

  //exports->Set(NanNew<String>("INT64_MIN"), NanNew<Number>(INT64_MIN));
  //exports->Set(NanNew<String>("INT64_MAX"), NanNew<Number>(INT64_MAX));
}

NODE_MODULE(ffmpeg, InitAll)
