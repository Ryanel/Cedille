#include <kernel/arch/i386/textmode_log.h>

void x86TextModeLog::DrawChar(int xpos, int ypos, char c, char attr) {
  int location = ((ypos * width) + xpos) * 2;
  buffer[location] = c;
  buffer[location + 1] = attr;
}

void x86TextModeLog::Scroll() {}

void x86TextModeLog::Clear() {
  x = 0;
  y = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      DrawChar(x, y, ' ', currentAttribute);
    }
  }
}

void x86TextModeLog::Newline() {
  y++;
  x = 0;
  Scroll();
}

void x86TextModeLog::ChangeForegroundColor(char c) {
  currentAttribute = (currentAttribute & 0x0F) ^ (c & 0xF0);
}
void x86TextModeLog::ChangeBackgroundColor(char c) {
  currentAttribute = (currentAttribute & 0xF0) ^ (c & 0x0F);
}
void x86TextModeLog::Print(char c) {
  if (c == '\n') {
    Newline();
    return;
  }
  DrawChar(x, y, c, currentAttribute);
  x++;
}