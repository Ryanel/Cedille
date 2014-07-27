#!/bin/bash
echo "Kernel Static Analyser|"
echo "----------------------/"

build_dir="$1"

kernel="$build_dir/kernel.elf"

echo "Assuming kernel is located at: $kernel"
file $kernel
