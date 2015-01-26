#include <nan.h>

#include "avformat/avformat.h"
#include "avcodec/avpicture.h"
#include "avcodec/avsubtitle.h"
#include "avcodec/avsubtitlerect.h"
//#include "avcodec/avcodec.h"
#include "avcodec/avpacket.h"
#include "avutil/avutil.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  //NanScope();

  avcodec_register_all();

  ffmpeg::avformat::AVFormat::Init(exports);

  ffmpeg::avcodec::AVPacket::Init(exports);

  ffmpeg::avcodec::AVSubtitle::Init(exports);
  ffmpeg::avcodec::AVSubtitleRect::Init(exports);
  ffmpeg::avcodec::AVPicture::Init(exports);

  ffmpeg::avutil::AVUtil::Init(exports);


  //FFmpeg::AVCodec::Initialize(exports);

  //exports->Set(NanNew<String>("INT64_MIN"), NanNew<Number>(INT64_MIN));
  //exports->Set(NanNew<String>("INT64_MAX"), NanNew<Number>(INT64_MAX));
}

NODE_MODULE(ffmpeg, InitAll)
