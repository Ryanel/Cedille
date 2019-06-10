#pragma once
#include <kernel/log.h>
#include <stdint.h>

class x86BGALog : public KernelLogImpl {
   private:
    int x = 0;
    int y = 0;

    unsigned int font_width = 8;
    unsigned int font_height = 8;
    unsigned char foreColor = 0xF;
    unsigned char backColor = 0x0;

    void DrawChar(int xpos, int ypos, char c, unsigned char fore,
                  unsigned char back);
    void Scroll();

   public:
    virtual void Init();
    virtual void Clear();
    virtual void Newline();
    virtual void ChangeForegroundColor(unsigned char c);
    virtual void ChangeBackgroundColor(unsigned char c);
    virtual void Print(char c);
    virtual void Scroll(int linesDown);
};