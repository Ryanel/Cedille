#include "kernel/log.h"
#include <stdio.h>
KernelLog g_log;

void KernelLog::Init(KernelLogImpl* impl) {
  this->impl = impl;
  impl->Clear();
}

void KernelLog::Log(char c) { impl->Print(c); }

void KernelLog::Log(const char* str) {
  statLoggedStatements++;
  printf("%s\n", str);
}

void KernelLog::ChangeBackgroundColor(char color) {
  impl->ChangeBackgroundColor(color);
}

void KernelLog::ChangeForegroundColor(char color) {
  impl->ChangeForegroundColor(color);
}

// C shim

extern "C" void kernel_log_char(char c) { g_log.Log(c); }