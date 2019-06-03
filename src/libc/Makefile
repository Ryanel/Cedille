DEFAULT_HOST!=../../scripts/default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../../scripts/target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -D__is_libc -Iinclude
CPPFLAGS:=$(CPPFLAGS) -ffreestanding -Wall -Wextra -D__is_libc -Iinclude
LIBK_CFLAGS:=$(CFLAGS) -D__is_libk
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

FREEOBJS=\
$(ARCH_FREEOBJS) \
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
stdlib/abort.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/strlen.o \
security/stack_smash_protector.o \

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libc.a: $(OBJS)
	@echo "[ ARC ] $(OBJS)"
	@$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	@echo "[ ARC ] $@"
	@$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	@echo "[  C  ] $<"
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.c.S:
	@echo "[  C  ] $<"
	@$(CC) -MD -c $< -o $@ $(CFLAGS)

.c.libk.o:
	@echo "[  C  ] $<"
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS)

.S.libk.o:
	@echo "[ ASM ] $<"
	@$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	@echo "[ INS ] libc headers..."
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	@echo "[ INS ] libc binaries..."
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
