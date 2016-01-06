#!/bin/sh

SCRIPT_DIR=$( (cd -P $(dirname $0) && pwd) )

FFMPEG_VER="2.8.4"
FFMPEG_PLAT="darwin"
FFMPEG_ARCH="x64"

FFMPEG_TAR="$SCRIPT_DIR/ffmpeg-$FFMPEG_VER.tar.bz2"
FFMPEG_ZIP="$SCRIPT_DIR/ffmpeg-$FFMPEG_VER-$FFMPEG_PLAT-$FFMPEG_ARCH.zip"

FFMPEG_SRC="$SCRIPT_DIR/ffmpeg"
FFMPEG_TMP="$SCRIPT_DIR/ffmpeg/ffmpeg-$FFMPEG_VER-tmp"
FFMPEG_LIB="$SCRIPT_DIR/ffmpeg/ffmpeg-$FFMPEG_VER-lib"

if [ ! -f $FFMPEG_TAR ]; then
  curl -O http://ffmpeg.org/releases/ffmpeg-$FFMPEG_VER.tar.bz2
fi
[ ! -f $FFMPEG_TAR ] && exit 1

if [ ! -d $FFMPEG_SRC ]; then
  mkdir -p $FFMPEG_SRC
  tar -xvjf $FFMPEG_TAR -C $FFMPEG_SRC
fi
[ ! -d $FFMPEG_SRC ] && exit 1

if [ ! -d $FFMPEG_TMP ]; then
  mkdir -p $FFMPEG_TMP
  pushd $FFMPEG_TMP
  $FFMPEG_SRC/ffmpeg-$FFMPEG_VER/configure \
    --prefix="$FFMPEG_LIB" \
    --enable-optimizations --enable-pic \
    --disable-doc --disable-logging --disable-debug --disable-stripping \
    --disable-postproc
  make && make install
  popd
fi

if [ ! -f $FFMPEG_ZIP ]; then
  pushd $FFMPEG_LIB
  zip -r $FFMPEG_ZIP .
  popd
fi
[ ! -f $FFMPEG_ZIP ] && exit 1
