#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./scripts/target-triplet-to-arch.sh $HOST) -cdrom build/artifact.iso -vga std -machine q35 -cpu coreduo -m 256M -boot once=d