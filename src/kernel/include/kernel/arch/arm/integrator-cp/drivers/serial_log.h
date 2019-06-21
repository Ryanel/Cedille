#pragma once
#include <kernel/log.h>
#include <stdint.h>

class SerialLogger : public KernelLogImpl {
   private:
    int x = 0;
    int y = 0;

    char foreColor = 0xF;
    char backColor = 0x0;

    void SendChar(char c);
    void SendString(const char* s);
    void Scroll();
    unsigned char AnsiEscapeColor(unsigned char termColor, bool background);
    unsigned char ConvertTermTo256(unsigned char termColor);

   public:
    virtual void Init();
    virtual void Clear();
    virtual void Newline();
    virtual void ChangeForegroundColor(unsigned char c);
    virtual void ChangeBackgroundColor(unsigned char c);
    virtual void Print(char c);
};