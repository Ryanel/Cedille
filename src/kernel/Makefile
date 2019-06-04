DEFAULT_HOST!=../../scripts/default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../../scripts/target-triplet-to-arch.sh $(HOST)

ASMFLAGS ?= -felf
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -D__is_kernel -Iinclude
CPPFLAGS:=$(CPPFLAGS) -ffreestanding -Wall -Wextra -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
	$(KERNEL_ARCH_OBJS) \
	main.o \
	log.o \
	panic.o

OBJS=\
	$(ARCHDIR)/crt/crti.o \
	$(ARCHDIR)/crt/crtbegin.o \
	$(KERNEL_OBJS) \
	$(ARCHDIR)/crt/crtend.o \
	$(ARCHDIR)/crt/crtn.o \

LINK_LIST=\
	$(LDFLAGS) \
	$(ARCHDIR)/crt/crti.o \
	$(ARCHDIR)/crt/crtbegin.o \
	$(KERNEL_OBJS) \
	$(LIBS) \
	$(ARCHDIR)/crt/crtend.o \
	$(ARCHDIR)/crt/crtn.o \

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S .asm .nasm

all: kernel.elf

kernel.elf: $(OBJS) $(ARCHDIR)/linker.ld
	@echo "[ ELF ] Compiling Kernel..."
	@$(CPP) -T $(ARCHDIR)/linker.ld -o $@ $(CPPFLAGS) $(LINK_LIST)
	@echo "[ ELF ] Kernel Compiled Successfully"
	

$(ARCHDIR)/crt/crtbegin.o $(ARCHDIR)/crt/crtend.o:
	@OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	@echo "[  C  ] $<"
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.cpp.o:
	@echo "[ CXX ] $<"
	@$(CPP) -MD -c $< -o $@ -std=gnu++11 $(CPPFLAGS)

.S.o:
	@echo "[ ASM ] $<"
	@$(CC) -MD -c $< -o $@ $(CFLAGS)

.nasm.o:
	@nasm $(ASMFLAGS) $< -o $@ 

clean:
	@echo "[CLEAN] src/kernel"
	@rm -f kernel.elf
	@rm -f $(OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
	@find . -name \*.expand -type f -delete
	@find . -name \*.vregs -type f -delete 
	@find . -name \*.into_cfglayout -type f -delete 
	@find . -name \*.jump -type f -delete 
	@find . -name \*.subreg* -type f -delete 
	@find . -name \*.dfinit -type f -delete 
	@find . -name \*.cse* -type f -delete 
	@find . -name \*.fwprop* -type f -delete 
	@find . -name \*.cprop* -type f -delete
	@find . -name \*.pre -type f -delete 
	@find . -name \*.ce* -type f -delete 
	@find . -name \*.reginfo -type f -delete
	@find . -name \*.loop* -type f -delete 
	@find . -name \*.dse* -type f -delete
	@find . -name \*.init-regs -type f -delete 
	@find . -name \*.combine -type f -delete 
	@find . -name \*.split* -type f -delete 

install: install-headers install-kernel

install-headers:
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)/kernel/
	@cp -R --preserve=timestamps include/kernel/. $(DESTDIR)$(INCLUDEDIR)/kernel/.

install-kernel: kernel.elf
	@echo "[ INS ] Installing Kernel to sysroot /boot"
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp kernel.elf $(DESTDIR)$(BOOTDIR)
	@wc -c kernel.elf

-include $(OBJS:.o=.d)
