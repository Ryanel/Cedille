SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./scripts/default-host.sh)}

export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc
export CPP=${HOST}-g++

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-Og -findirect-inlining -g -fstack-protector-strong'
export CPPFLAGS='-Og -findirect-inlining -g -fstack-protector-strong -fno-exceptions -fno-rtti'

# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/build/sysroot"
export CC="$CC --sysroot=$SYSROOT"
export CPP="$CPP --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
#if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
#  export CC="$CC -isystem=$INCLUDEDIR"
#  export CPP="$CPP -isystem=$INCLUDEDIR"
#fi

export CC="$CC -isystem=$INCLUDEDIR"
export CPP="$CPP -isystem=$INCLUDEDIR"