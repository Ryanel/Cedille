#=================================================================================================
#Cédille Build System
#=================================================================================================
ARCH :=x86
ARCH_DIRECTORY := kernel/arch/${ARCH}

ARCH_TOOLKIT_PREFIX := 

BUILD_DIRECTORY := build

COMPILE_OPTIONS := -DDEBUG -DARCH=${ARCH} -DARCH_STRING="\"${ARCH}\"" #-D KERNEL_SYMBOLS #-D KLOG_TITLE_TIME
#Files
BOOT_FILES := $(patsubst %.c,%.o,$(wildcard kernel/init/*.c))
ARCH_BOOT_FILES := $(patsubst %.s,%.o,$(wildcard ${ARCH_DIRECTORY}/init/*.s)) $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/init/*.c))
ARCH_LOW_FILES := $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/low/*.c)) $(patsubst %.s,%.o,$(wildcard ${ARCH_DIRECTORY}/low/*.s))
LIB_FILES := $(patsubst %.c,%.o,$(wildcard kernel/lib/*.c))
ARCH_LIB_FILES := $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/lib/*.c))
DRIVER_FILES := $(patsubst %.c,%.o,$(wildcard kernel/drivers/*.c))
ARCH_DRIVER_FILES  := $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/drivers/*.c))
KERNEL_FILES := $(patsubst %.c,%.o,$(wildcard kernel/*.c))
ARCH_FILES := $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/*.c))$(patsubst %.s,%.o,$(wildcard ${ARCH_DIRECTORY}/*.s))
FS_FILES := $(patsubst %.c,%.o,$(wildcard kernel/fs/*.c))
SRC_FILES := ${BOOT_FILES} ${KERNEL_FILES} ${DRIVER_FILES} ${LIB_FILES} ${ARCH_FILES} ${ARCH_BOOT_FILES} ${ARCH_LOW_FILES} ${ARCH_LIB_FILES} ${ARCH_DRIVER_FILES} ${FS_FILES}

KASM_FILES := $(patsubst %.s,%.o,$(wildcard ${ARCH_DIRECTORY}asm/*.s))

#Compiler Options
CC:=clang -DX86 -target i586-elf
CPP:=clang++
STRIP:= strip
NM := nm
C_OPTIONS := -ffreestanding -std=gnu99  -nostartfiles 
C_OPTIONS += -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
C_OPTIONS += -Wno-unused-function
C_OPTIONS += -s -g -Os
C_OPTIONS += -z max-page-size=0x1000
LD := ./toolkit/binutils/bin/i586-elf-ld -m elf_i386
LFLAGS :=  -lgcc -nostdlib
LD_SCRIPT := ${ARCH_DIRECTORY}/link.ld
INCLUDE_DIR := "./kernel/includes"
CROSS_CLANG := -target i586-elf
ASM := nasm -felf 
GENISO := xorriso -as mkisofs
EMU := qemu-system-i386
#Rules
.PHONY: iso clean

all: build-dir kernel gen-symbols add-symbols strip iso

build-dir:
	@echo "DIR    | ${BUILD_DIRECTORY}"
	@mkdir -p ${BUILD_DIRECTORY}

arch-boot: ${ARCH_BOOT_FILES}
boot: ${BOOT_FILES}

arch-low: ${ARCH_LOW_FILES}

lib: ${LIB_FILES}
arch-lib: ${ARCH_LIB_FILES}

arch-files: ${ARCH_FILES}

drivers: ${DRIVER_FILES}
arch-drivers: ${ARCH_DRIVER_FILES}

fs: ${FS_FILES}
	
kernel: arch-boot boot lib drivers arch-files arch-low arch-lib arch-drivers fs ${KERNEL_FILES}
	@echo " LD [K]| kernel.elf"
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o ${BUILD_DIRECTORY}/kernel.elf ${SRC_FILES}

kernel-sparc: arch-boot arch-files arch-low arch-drivers lib arch-lib
	@echo " LD [K]| kernel.aout (sparc)"
	@${LD} -T ${LD_SCRIPT} -o ${BUILD_DIRECTORY}/kernel.aout ${ARCH_BOOT_FILES} ${ARCH_FILES} ${ARCH_LOW_FILES} ${ARCH_DRIVER_FILES} ${LIB_FILES} ${ARCH_LIB_FILES}

asm-src: ${KASM_FILES}

kernel-asm: asm-src
	@echo " LD [K]| kernel.elf (x86_asm)"
	@${LD} ${LFLAGS} -T ${ARCH_DIRECTORY}asm/link.ld -o ${BUILD_DIRECTORY}/kernel-asm.elf ${KASM_FILES}
#Generic

%.o: %.s
	@echo " AS    |" $@
	@${ASM} -o $@ $<

%.o: %.c
	@echo " CC    |" $@
	@${CC} -c ${C_OPTIONS} ${COMPILE_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

%.o: %.cpp
	@echo " CX    |" $@
	@${CPP} -c ${CPP_OPTIONS} ${COMPILE_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

clean: prep-dist
	@echo "CLN    | *.o" 
	@-find . -name "*.o" -type f -delete

prep-dist:
	@-rm -rf *~ doc/* kernel/*~

run:
	@${EMU} -m 4 -serial stdio -cdrom ${BUILD_DIRECTORY}/cdrom.iso

strip:
	@echo "STRIP  | "${BUILD_DIRECTORY}/kernel.elf
	@${STRIP} ${BUILD_DIRECTORY}/kernel.elf
sparc-iso:
	@echo "GEN [A]| ${BUILD_DIRECTORY}/sparc-bootblock.bin"
	@dd if=/dev/zero of=${BUILD_DIRECTORY}/sparc-bootblock.bin bs=2048 count=4
	@dd if=${BUILD_DIRECTORY}/kernel.aout of=${BUILD_DIRECTORY}/bootblock.bin bs=512 seek=1 conv=notrunc
	@echo "ISO [A]| ${BUILD_DIRECTORY}/sparc-iso.iso"
	@${GENISO} -quiet -o ${BUILD_DIRECTORY}/sparc-iso.iso -G ${BUILD_DIRECTORY}/bootblock.bin iso
iso:
	@echo "ISO [A]| ${BUILD_DIRECTORY}/cdrom/iso"
	@cp ${BUILD_DIRECTORY}/kernel.elf iso/kernel.elf
	@${GENISO} -R -b boot/grub/stage2_eltorito -quiet -no-emul-boot -boot-load-size 4 -boot-info-table -o ${BUILD_DIRECTORY}/cdrom.iso iso

gen-symbols:
	@echo "GEN [M]| ${BUILD_DIRECTORY}/kernel.elf -> ${BUILD_DIRECTORY}/kernel.map"
	@${NM} ${BUILD_DIRECTORY}/kernel.elf > ${BUILD_DIRECTORY}/kernel.map
add-symbols: gen-symbols
	@echo " CP [M]| ${BUILD_DIRECTORY}/kernel.map -> iso/boot/symbols.mod"
	@cp ${BUILD_DIRECTORY}/kernel.map iso/boot/symbols.mod
x86:
	@make all iso
x64:
	@make ASM="nasm -felf64" LD="${ARCH_TOOLKIT_PREFIX}-ld" LFLAGS="" CC="${ARCH_TOOLKIT_PREFIX}-gcc -DX64" C_OPTIONS="${C_OPTIONS} -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow" ARCH=x64 all iso 
arm:
	@make integrator-cp
integrator-cp:
	@make build-dir kernel ARCH=arm/integrator-cp ASM=arm-none-eabi-as LD="arm-none-eabi-gcc -lgcc -ffreestanding -nostartfiles -fno-builtin -nostartfiles" LFLAGS="" CC="arm-none-eabi-gcc -DARM"
rpi:
	@make build-dir kernel gen-symbols add-symbols ARCH=arm/rpi ASM=arm-none-eabi-as LD="arm-none-eabi-gcc -lgcc -nostartfiles -fno-builtin -nostartfiles -DARCH_ARM_RPI" LFLAGS="" CC="arm-none-eabi-gcc -DARM"
run-arm-icp:
	@qemu-system-arm -M integratorcp -serial stdio -kernel ${BUILD_DIRECTORY}/kernel.elf -nographic -monitor none -initrd iso/boot/initrd.img
run-arm-rpi:
	@qemu-system-arm -m 8 -M versatilepb -cpu arm1176 -serial stdio -kernel ${BUILD_DIRECTORY}/kernel.elf -initrd iso/boot/initrd.img
run-sparc:
	@qemu-system-sparc -serial stdio -cdrom build-sparc/sparc-iso.iso
sparc:
	@make ARCH=sparc ASM=sparc-elf-as LD=sparc-elf-ld CC="sparc-elf-gcc -DSPARC" BUILD_DIRECTORY=build-sparc build-dir kernel-sparc sparc-iso
run-asm:
	@qemu-system-i386 -m 8 -serial stdio -kernel ${BUILD_DIRECTORY}/kernel-asm.elf
