#ifndef SRC_ADDONS_AVUTIL_SAMPLEFMT_H_
#define SRC_ADDONS_AVUTIL_SAMPLEFMT_H_

#include <nan.h>

extern "C" {
#include "libavutil/samplefmt.h"
}

namespace ffmpeg {
namespace avutil {

class AVSampleFormat {
 public:
  static void Init(v8::Handle<v8::Object> exports);
 private:
  static NAN_METHOD(GetSampleFmtName);
  static NAN_METHOD(GetSampleFmt);
  static NAN_METHOD(GetAltSampleFmt);
  static NAN_METHOD(GetPackedSampleFmt);
  static NAN_METHOD(GetPlanarSampleFmt);
  static NAN_METHOD(GetSampleFmtString);
  static NAN_METHOD(GetBytesPerSample);
  static NAN_METHOD(IsSampleFmtPlanar);
  static NAN_METHOD(GetSamplesBufferSize);
};

}  // namespace avutil
}  // namespace ffmpeg

#endif  // SRC_ADDONS_AVUTIL_SAMPLEFMT_H_
