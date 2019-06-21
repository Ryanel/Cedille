#!/bin/sh
set -e
. ./build.sh
qemu-system-arm -kernel build/sysroot/boot/kernel.elf -machine integratorcp -m 128 -serial stdio
