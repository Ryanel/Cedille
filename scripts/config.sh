#!/bin/bash
SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./scripts/default-host.sh)}
export PLATFORM=${PLATFORM:-pc}
export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc
export CPP=${HOST}-g++
export STRIP=${HOST}-strip

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-Og -g -fstack-protector'
export CPPFLAGS='-Og -g -fstack-protector -fno-exceptions -fno-rtti'

# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/build/sysroot"
export CC="$CC --sysroot=$SYSROOT -isystem=$INCLUDEDIR"
export CPP="$CPP --sysroot=$SYSROOT -isystem=$INCLUDEDIR"

CONVERTED_ARCH=$(./scripts/target-triplet-to-arch.sh $HOST)

echo "[CONFIG] $HOST ($CONVERTED_ARCH) on $PLATFORM"
