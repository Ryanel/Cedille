#!/bin/bash
set -e
. ./build.sh

mkdir -p ./build/isodir
mkdir -p ./build/isodir/boot
mkdir -p ./build/isodir/boot/grub

cp -r ./build/sysroot/* ./build/isodir/

cat > ./build/isodir/boot/grub/grub.cfg << EOF
set default=0
set timeout=0
menuentry "Cedille" {
	multiboot /boot/kernel.elf
}
EOF

grub-mkrescue -o build/artifact.iso ./build/isodir
