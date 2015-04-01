#!/usr/bin/env python

import contextlib
import os
import subprocess
import sys


FFMPEG_VER = '2.6'
SOURCE_ROOT = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


def main():
  os.chdir(SOURCE_ROOT)

  update_submodules()
  build_ffmpeg()

def update_submodules():
  execute(['git', 'submodule', 'sync'])
  execute(['git', 'submodule', 'update', '--init', '--recursive'])

def build_ffmpeg():
  source = os.path.join(SOURCE_ROOT, 'src', 'ffmpeg')
  object = os.path.join(SOURCE_ROOT, 'tmp', 'ffmpeg-obj')
  target = os.path.join(SOURCE_ROOT, 'tmp', 'ffmpeg-lib')
  with scoped_cwd(source):
    execute(['git', 'checkout', 'n%s' % FFMPEG_VER])
  if not os.path.exists(object):
    os.makedirs(object)
  with scoped_cwd(object):
    execute(['%s/configure' % source, '--prefix=%s' % target, '--enable-shared'])
    execute(['make'])
    execute(['make', 'install'])

def execute(argv):
  try:
    return subprocess.check_output(argv, stderr=subprocess.STDOUT)
  except subprocess.CalledProcessError as e:
    print e.output
    raise e

@contextlib.contextmanager
def scoped_cwd(path):
  cwd = os.getcwd()
  os.chdir(path)
  try:
    yield
  finally:
    os.chdir(cwd)


if __name__ == '__main__':
  sys.exit(main())
