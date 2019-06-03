#include <stdio.h>
#include <stdlib.h>

#include <kernel/arch/i386/textmode_log.h>
#include <kernel/log.h>
#include <kernel/main.h>
x86TextModeLog textmodeLog;

extern "C" void kernel_start(void) {
  g_log.Init(&textmodeLog);
  g_log.Log("Hello, World!");
  kmain();
}
