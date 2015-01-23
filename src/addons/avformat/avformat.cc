#include "avformat/avformat.h"
#include "avformat/avformatcontext.h"
#include "avformat/avinputformat.h"
#include "avformat/avoutputformat.h"
#include "avformat/avstream.h"
#include "avformat/avprogram.h"
#include "avformat/avchapter.h"
#include "avcodec/avcodec.h"
#include "avcodec/avcodeccontext.h"
#include "avcodec/avpacket.h"
#include "avutil/avutil.h"

using namespace node;
using namespace v8;


inline void
SetContant(Handle<Object> const &target, const char *symbol, int value) {
  target->Set(NanNew<String>(symbol), NanNew<Number>(value));
}


void FFmpeg::AVFormat::Initialize(Handle<Object> target) {
  NanScope();

  AVOutputFormatWrapper::Initialize(target);
  AVInputFormatWrapper::Initialize(target);
  AVStreamWrapper::Initialize(target);
  AVProgramWrapper::Initialize(target);
  AVChapterWrapper::Initialize(target);
  AVFormatContextWrapper::Initialize(target);

  // libavformat/avformat.h
  SetContant(target, "AVFMT_NOFILE", AVFMT_NOFILE);
  SetContant(target, "AVFMT_NEEDNUMBER", AVFMT_NEEDNUMBER);
  SetContant(target, "AVFMT_SHOW_IDS", AVFMT_SHOW_IDS);
  SetContant(target, "AVFMT_RAWPICTURE", AVFMT_RAWPICTURE);
  SetContant(target, "AVFMT_GLOBALHEADER", AVFMT_GLOBALHEADER);
  SetContant(target, "AVFMT_NOTIMESTAMPS", AVFMT_NOTIMESTAMPS);
  SetContant(target, "AVFMT_GENERIC_INDEX", AVFMT_GENERIC_INDEX);
  SetContant(target, "AVFMT_TS_DISCONT", AVFMT_TS_DISCONT);
  SetContant(target, "AVFMT_VARIABLE_FPS", AVFMT_VARIABLE_FPS);
  SetContant(target, "AVFMT_NODIMENSIONS", AVFMT_NODIMENSIONS);
  SetContant(target, "AVFMT_NOSTREAMS", AVFMT_NOSTREAMS);
  SetContant(target, "AVFMT_NOBINSEARCH", AVFMT_NOBINSEARCH);
  SetContant(target, "AVFMT_NOGENSEARCH", AVFMT_NOGENSEARCH);
  SetContant(target, "AVFMT_NO_BYTE_SEEK", AVFMT_NO_BYTE_SEEK);
  SetContant(target, "AVFMT_ALLOW_FLUSH", AVFMT_ALLOW_FLUSH);
  SetContant(target, "AVFMT_TS_NONSTRICT", AVFMT_TS_NONSTRICT);
  SetContant(target, "AVFMT_TS_NEGATIVE", AVFMT_TS_NEGATIVE);
  SetContant(target, "AVFMT_SEEK_TO_PTS", AVFMT_SEEK_TO_PTS);

  // libavformat/avformat.h
  SetContant(target, "AVFMT_FLAG_GENPTS", AVFMT_FLAG_GENPTS);
  SetContant(target, "AVFMT_FLAG_IGNIDX", AVFMT_FLAG_IGNIDX);
  SetContant(target, "AVFMT_FLAG_NONBLOCK", AVFMT_FLAG_NONBLOCK);
  SetContant(target, "AVFMT_FLAG_IGNDTS", AVFMT_FLAG_IGNDTS);
  SetContant(target, "AVFMT_FLAG_NOFILLIN", AVFMT_FLAG_NOFILLIN);
  SetContant(target, "AVFMT_FLAG_NOPARSE", AVFMT_FLAG_NOPARSE);
  SetContant(target, "AVFMT_FLAG_NOBUFFER", AVFMT_FLAG_NOBUFFER);
  SetContant(target, "AVFMT_FLAG_CUSTOM_IO", AVFMT_FLAG_CUSTOM_IO);
  SetContant(target, "AVFMT_FLAG_DISCARD_CORRUPT", AVFMT_FLAG_DISCARD_CORRUPT);
  SetContant(target, "AVFMT_FLAG_FLUSH_PACKETS", AVFMT_FLAG_FLUSH_PACKETS);
  SetContant(target, "AVFMT_FLAG_MP4A_LATM", AVFMT_FLAG_MP4A_LATM);
  SetContant(target, "AVFMT_FLAG_SORT_DTS", AVFMT_FLAG_SORT_DTS);
  SetContant(target, "AVFMT_FLAG_PRIV_OPT", AVFMT_FLAG_PRIV_OPT);
  SetContant(target, "AVFMT_FLAG_KEEP_SIDE_DATA", AVFMT_FLAG_KEEP_SIDE_DATA);
}
