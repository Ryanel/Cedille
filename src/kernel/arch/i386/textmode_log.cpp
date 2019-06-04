#include <kernel/arch/i386/textmode_log.h>
#include <stdint.h>

void x86TextModeLog::DrawChar(int xpos, int ypos, char c, char fore,
                              char back) {
    int location = ((ypos * width) + xpos) * 2;
    uint8_t attrib = (back << 4) | (fore & 0x0F);
    buffer[location] = c;
    buffer[location + 1] = attrib;
}

void x86TextModeLog::Scroll() {}

void x86TextModeLog::Clear() {
    x = 0;
    y = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            DrawChar(x, y, ' ', foreColor, backColor);
        }
    }

    x = left_margin;
}

void x86TextModeLog::Newline() {
    // Fill blank space with attribute...
    for (int i = x; i < width; i++) {
        DrawChar(i, y, ' ', foreColor, backColor);
    }
    // Reset and scroll
    y++;
    x = left_margin;
    for (int i = 0; i < left_margin; i++) {
        DrawChar(i, y, ' ', foreColor, backColor);
    }
    Scroll();
}

void x86TextModeLog::ChangeForegroundColor(char c) {
    foreColor = c;
}
void x86TextModeLog::ChangeBackgroundColor(char c) {
    backColor = c;
}
void x86TextModeLog::Print(char c) {
    if (c == '\n') {
        Newline();
        return;
    }
    DrawChar(x, y, c, foreColor, backColor);
    x++;
}