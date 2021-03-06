#pragma once
#include <kernel/log.h>
#include <stdint.h>

class SerialLog : public KernelLogImpl {
   private:
    int x = 0;
    int y = 0;

    char foreColor = 0xF;
    char backColor = 0x0;

    void SendChar(char c);
    void Scroll();

   public:
    virtual void Init();
    virtual void Clear();
    virtual void Newline();
    virtual void ChangeForegroundColor(unsigned char c);
    virtual void ChangeBackgroundColor(unsigned char c);
    virtual void Print(char c);
};