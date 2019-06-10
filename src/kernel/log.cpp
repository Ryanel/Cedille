#include "kernel/log.h"
#include <stdio.h>
#include <string.h>

KernelLog g_log;

const unsigned char log_severity_table[] = {
    // Fore, Back
    0x08, 0xFF,  // Debug
    0x0F, 0xFF,  // Message (Default)
    0x0B, 0xFF,  // Info
    0x0D, 0xFF,  // Notice
    0x0E, 0x08,  // Warn
    0x0C, 0x08,  // Error
    0x04, 0x0F,  // Panic
};

void KernelLog::Init(KernelLogImpl* impl) {
    this->impl = impl;

    defaultColor.fore = 0x0F;
    defaultColor.back = 0x00;

    impl->Init();
    impl->Clear();

    renderer.Init();
    renderer.SignalFullRender();
}

void KernelLog::Log(char c) {
    impl->Print(c);
}

LogColor KernelLog::GetSeverityColor(unsigned int severity) {
    LogColor c;
    c.fore = log_severity_table[severity * 2];
    c.back = log_severity_table[(severity * 2) + 1];
    return c;
}

void KernelLog::Log(LogEntry newEntry) {
    // Attempt to insert message
    AppendEntry(newEntry);
    renderer.SignalNewEntry();
}

void KernelLog::Log(const char* str) {
    Log(LOG_MESSAGE, "", str);
}

void KernelLog::Log(const char* tag, const char* str) {
    Log(LOG_MESSAGE, tag, str);
}

void KernelLog::Log(unsigned int severity, const char* tag, const char* fmt,
                    ...) {
    LogEntry newEntry;
    newEntry.severity = severity;
    newEntry.tag = tag;
    newEntry.color = currentColor;

    char buffer[LOG_MAX_MESSAGE_SIZE];

    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);

    strncpy(newEntry.message, buffer, LOG_MAX_MESSAGE_SIZE);
    newEntry.message[LOG_MAX_MESSAGE_SIZE - 1] = 0;  // Force null character

    Log(newEntry);
}

void KernelLog::SetBackgroundColor(char color) {
    currentColor.back = color;
    impl->ChangeBackgroundColor(color);
}

void KernelLog::SetForegroundColor(char color) {
    currentColor.fore = color;
    impl->ChangeForegroundColor(color);
}

void KernelLog::SetColors(LogColor colors) {
    SetForegroundColor(colors.fore);
    SetBackgroundColor(colors.back);
}

void KernelLog::AppendEntry(LogEntry entry) {
    if (log_entry_index >= LOG_MAX_ENTRIES - 1) {
        // Shift all entries down by one, discarding the oldest...
        for (int i = 1; i < LOG_MAX_ENTRIES - 1; i++) {
            entry_buffer[i - 1] = entry_buffer[i];
        }
        log_entry_index--;
    }

    entry_buffer[log_entry_index++] = entry;
}

// C shim
extern "C" void kernel_log_char(char c) {
    g_log.Log(c);
}