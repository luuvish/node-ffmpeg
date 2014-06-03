#!/usr/bin/env python

import contextlib
import os
import subprocess
import sys


SOURCE_ROOT = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


def main():
  os.chdir(SOURCE_ROOT)

  update_submodules()
  build_ffmpeg()


def update_submodules():
  execute(['git', 'submodule', 'sync'])
  execute(['git', 'submodule', 'update', '--init', '--recursive'])


def build_ffmpeg():
  source = os.path.join(SOURCE_ROOT, 'vendor', 'ffmpeg')
  build = os.path.join(SOURCE_ROOT, 'tmp', 'ffmpeg-build')
  target = os.path.join(SOURCE_ROOT, 'tmp', 'ffmpeg')
  with scoped_cwd(source):
    execute(['git', 'checkout', 'n%s' % '2.2.3'])
  if not os.path.exists(build):
    os.makedirs(build)
  with scoped_cwd(build):
    execute(['%s/configure' % source, '--prefix=%s' % target])
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
