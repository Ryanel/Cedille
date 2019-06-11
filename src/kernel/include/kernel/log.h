#pragma once

#define LOG_DEBUG 0
#define LOG_MESSAGE 1
#define LOG_INFO 2
#define LOG_NOTICE 3
#define LOG_WARN 4
#define LOG_ERROR 5
#define LOG_PANIC 6

#define LOG_MAX_MESSAGE_SIZE 128  // Maximum message size for message
#define LOG_MAX_ENTRIES 200       // 2 screens of entries...

struct LogColor {
    unsigned char fore;
    unsigned char back;
};

struct LogEntry {
    unsigned char severity;
    const char* tag;
    char message[LOG_MAX_MESSAGE_SIZE];
    LogColor color;
};

class KernelLogImpl {
   public:
    virtual void Init() {
        width = 80;
        height = 25;
    }
    virtual void Clear() = 0;
    virtual void Newline() = 0;
    virtual void ChangeForegroundColor(unsigned char c) = 0;
    virtual void ChangeBackgroundColor(unsigned char c) = 0;
    virtual void Print(char c) = 0;
    virtual void Scroll(int linesDown) {
        linesDown = linesDown;
    }
    unsigned int left_margin = 0;

    unsigned int width;
    unsigned int height;
};

class BootLogRenderer {
   private:
    bool requestFullRender = false;
    bool scrollAppend = false;
    int log_tag_width = 6;
    int log_scroll_previous = 0;
    int log_scroll_current = 0;

    void PrintEntry(LogEntry e);
    void PrintTag(const char* tag);
    void PrintMessage(const char* msg);

    void UpdateRender();
    void StateFull();
    void StateAppend();
    void StateScrollUp();
    void StateScrollDown();
    enum RenderState { Append,
                       ScrollUp,
                       ScrollDown,
                       Full };

   public:
    void Init();
    void SignalNewEntry();
    void SignalFullRender();
};

class KernelLog {
   private:
    KernelLogImpl* impl;
    BootLogRenderer renderer;
    // Log Entry Stuff
    LogEntry entry_buffer[LOG_MAX_ENTRIES];
    unsigned int log_entry_index = 0;

    // Coloring
    LogColor currentColor;
    LogColor defaultColor;
    LogColor GetSeverityColor(unsigned int severity);
    void SetForegroundColor(char c);
    void SetBackgroundColor(char c);

    // Printing...

    void AppendEntry(LogEntry entry);
    void Log(LogEntry entry);

   public:
    void Init(KernelLogImpl* impl);

    void Log(char c);
    void Log(const char* str);
    void Log(const char* tag, const char* str);
    // void Log(unsigned int severity, const char* tag, const char* str);
    void Log(unsigned int severity, const char* tag, const char* str, ...);

    void SetColors(LogColor colors);

    friend class BootLogRenderer;
};

extern KernelLog g_log;