#pragma once

#define LOG_DEBUG 0
#define LOG_MESSAGE 1
#define LOG_INFO 2
#define LOG_NOTICE 3
#define LOG_WARN 4
#define LOG_ERROR 5
#define LOG_PANIC 6

class KernelLogImpl {
   public:
    virtual void Clear() = 0;
    virtual void Newline() = 0;
    virtual void ChangeForegroundColor(char c) = 0;
    virtual void ChangeBackgroundColor(char c) = 0;
    virtual void Print(char c) = 0;
    int left_margin = 0;
};

class KernelLog {
   private:
    int statLoggedStatements = 0;
    int log_tag_width = 6;
    KernelLogImpl* impl;

    char SetSeverityColor(unsigned int severity);
    void PrintTag(const char* tag);
    void PrintMessage(const char* msg);
    char foreColor = 0xF;
    char backColor = 0x0;

   public:
    void Init(KernelLogImpl* impl);
    void Log(const char* str);
    void Log(const char* tag, const char* str);
    void Log(unsigned int severity, const char* tag, const char* str);
    void Log(char c);
    void Print(const char* str);
    void ChangeForegroundColor(char c);
    void ChangeBackgroundColor(char c);
};

extern KernelLog g_log;