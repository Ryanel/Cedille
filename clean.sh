#!/bin/sh
set -e
. ./scripts/config.sh

for PROJECT in $PROJECTS; do
  (cd src/$PROJECT && $MAKE clean)
done

echo "[CLEAN] build/"

rm -rf ./build/
