#Configuration
#--------------------------------------------
ARCH := x86
BOARD := generic

BOARD_ID := ${ARCH}/${BOARD}
COMPILE_OPTIONS := -DDEBUG


#Tools
#--------------------------------------------

AS := nasm -felf 
CC:=clang -target i586-elf
STRIP:= strip
NM := nm
LD := ./toolkit/binutils/bin/i586-elf-ld -m elf_i386

C_OPTIONS := -ffreestanding -std=gnu99  -nostartfiles 
C_OPTIONS += -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
C_OPTIONS += -Wno-unused-function
C_OPTIONS += -s -g -Os

LD_SCRIPT := kernel/arch/${ARCH}/${BOARD}/link.ld
INCLUDE_DIR := "kernel/includes"

GENISO := xorriso -as mkisofs
EMU := qemu-system-i386


#FILES
#--------------------------------------------

GLOBAL_ROOT_FILES := $(patsubst %.c,%.o,$(wildcard kernel/*.c))
GLOBAL_INIT_FILES := $(patsubst %.c,%.o,$(wildcard kernel/init/*.c))
GLOBAL_DRIVERS_FILES := $(patsubst %.c,%.o,$(wildcard kernel/drivers/*.c))
GLOBAL_LOW_FILES := $(patsubst %.c,%.o,$(wildcard kernel/low/*.c))
GLOBAL_FS_FILES := $(patsubst %.c,%.o,$(wildcard kernel/fs/*.c))
GLOBAL_LIB_FILES := $(patsubst %.c,%.o,$(wildcard kernel/lib/*.c))
GLOBAL_TEST_FILES := $(patsubst %.c,%.o,$(wildcard kernel/test/*.c))

ARCH_ROOT_FILES := $(patsubst %.s,%.o,$(wildcard kernel/arch/${ARCH}/*.s)) $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/*.c))
ARCH_DRIVERS_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/drivers/*.c))
ARCH_LOW_FILES := $(patsubst %.s,%.o,$(wildcard kernel/arch/${ARCH}/low/*.s))
ARCH_FS_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/fs/*.c))
ARCH_LIB_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/lib/*.c))
ARCH_TEST_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/test/*.c))

BOARD_ROOT_FILES := $(patsubst %.s,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/*.s)) $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/*.c))
BOARD_INIT_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/init/*.c)) $(patsubst %.s,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/init/*.s))
BOARD_DRIVER_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/drivers/*.c))
BOARD_LOW_FILES := $(patsubst %.s,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/low/*.s))
BOARD_FS_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/fs/*.c))
BOARD_LIB_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/lib/*.c))
BOARD_TEST_FILES := $(patsubst %.c,%.o,$(wildcard kernel/arch/${ARCH}/${BOARD}/test/*.c))

#Canonicate them together
GLOBAL_FILES := ${GLOBAL_ROOT_FILES} ${GLOBAL_INIT_FILES} ${GLOBAL_DRIVERS_FILES} ${GLOBAL_LOW_FILES} ${GLOBAL_FS_FILES} ${GLOBAL_LIB_FILES} ${GLOBAL_TEST_FILES}
ARCH_FILES := ${ARCH_ROOT_FILES} ${ARCH_DRIVERS_FILES} ${ARCH_LOW_FILES} ${ARCH_FS_FILES} ${ARCH_LIB_FILES} ${ARCH_TEST_FILES}
BOARD_FILES := ${BOARD_ROOT_FILES} ${BOARD_INIT_FILES} ${BOARD_DRIVER_FILES} ${BOARD_LOW_FILES} ${BOARD_FS_FILES} ${BOARD_LIB_FILES} ${BOARD_TEST_FILES}

ALL_SOURCE_FILES := ${GLOBAL_FILES} ${ARCH_FILES} ${BOARD_FILES}
#RULES
#--------------------------------------------

all: kernel gen-symbols

kernel: ${GLOBAL_FILES} arch board
	@echo " LD [K]| kernel.elf"
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o build/kernel.elf ${ALL_SOURCE_FILES}
arch: ${ARCH_FILES}

board: ${BOARD_FILES}

%.o: %.s
	@echo " AS     " $@
	@${AS} -o $@ $<

%.o: %.c
	@echo " CC     " $@
	@${CC} -c ${C_OPTIONS} ${COMPILE_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

clean:
	@echo "CLN    | *.o" 
	@-find . -name "*.o" -type f -delete

iso: kernel
	@echo "ISO [A]| build/cdrom.iso"
	@cp ${BUILD_DIRECTORY}/kernel.elf iso/kernel.elf
	@${GENISO} -R -b boot/grub/stage2_eltorito -quiet -no-emul-boot -boot-load-size 4 -boot-info-table -o ${BUILD_DIRECTORY}/cdrom.iso iso

gen-symbols: kernel
	@echo "GEN [M]| build/kernel.elf -> build/kernel.map"
	@${NM} build/kernel.elf > build/kernel.map
