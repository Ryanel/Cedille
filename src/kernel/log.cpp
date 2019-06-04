#include "kernel/log.h"
#include <stdio.h>
#include <string.h>
KernelLog g_log;

char log_severity_table[] = {
    0x08,  // Debug
    0x0F,  // Message (Default)
    0x0D,  // Info
    0x0B,  // Notice
    0x0E,  // Warn
    0x0C,  // Error
    0x04,  // Panic
};

void KernelLog::Init(KernelLogImpl* impl) {
    this->impl = impl;
    log_tag_width = 10;  // Set initial margin...
    impl->Clear();
}

void KernelLog::Log(char c) {
    impl->Print(c);
}

void KernelLog::Print(const char* s) {
    printf("%s", s);
}

char KernelLog::SetSeverityColor(unsigned int severity) {
    if (severity >= sizeof(log_severity_table)) {
        severity = sizeof(log_severity_table) - 1;
    }
    char old_foreColor = foreColor;
    ChangeForegroundColor(log_severity_table[severity]);
    return old_foreColor;
}

void KernelLog::PrintTag(const char* tag) {
    // How much padding between tag and layout characters
    const int padding_size = 2;

    int tag_len = strlen(tag);
    int tag_padding_len = tag_len + padding_size;

    // If tag + pad length is bigger then what we've reserved, expand
    if (tag_padding_len > log_tag_width) {
        log_tag_width = tag_padding_len;
    }

    // Print right justified
    int right_space_width = log_tag_width - tag_padding_len;
    for (int i = 0; i < right_space_width; i++) {
        Log(' ');
    }
    printf("%s: ", tag);
}

void KernelLog::PrintMessage(const char* str) {
    // Set left margin...
    impl->left_margin = log_tag_width;
    // Print
    printf("%s", str);
    // Reset left margin
    impl->left_margin = 0;
}
void KernelLog::Log(const char* str) {
    // Print an empty tag
    PrintTag("");

    PrintMessage(str);

    impl->Print('\n');  // Print newline
    statLoggedStatements++;
}

void KernelLog::Log(const char* tag, const char* str) {
    char old_foreColor = SetSeverityColor(LOG_MESSAGE);

    PrintTag(tag);
    PrintMessage(str);
    impl->Print('\n');  // Print newline

    ChangeForegroundColor(old_foreColor);  // Reset forecolor

    statLoggedStatements++;
}

void KernelLog::Log(unsigned int severity, const char* tag, const char* str) {
    char old_foreColor = SetSeverityColor(severity);
    PrintTag(tag);
    PrintMessage(str);
    impl->Print('\n');  // Print newline

    ChangeForegroundColor(old_foreColor);  // Reset forecolor

    statLoggedStatements++;
}

void KernelLog::ChangeBackgroundColor(char color) {
    impl->ChangeBackgroundColor(color);
    backColor = color;
}

void KernelLog::ChangeForegroundColor(char color) {
    impl->ChangeForegroundColor(color);
    foreColor = color;
}

// C shim

extern "C" void kernel_log_char(char c) {
    g_log.Log(c);
}