#!/bin/sh
set -e
. ./sysroot.sh

for PROJECT in $PROJECTS; do
  (cd ./src/$PROJECT && DESTDIR="$SYSROOT" $MAKE install $1)
done
