#include <kernel/arch/i386/drivers/textmode_log.h>
#include <kernel/arch/i386/ports.h>
#include <stdint.h>
#include <string.h>

void x86TextModeLog::Init() {
    // Disable blinking
    inb(0x03DA);                     // Set to index state
    outb(0x03C0, 0x30);              // Write index to address mode control register
    char regcontents = inb(0x03C1);  // Get contents
    regcontents &= 0xF7;             // Unset bit 3 (blink)
    outb(0x03C0, regcontents);       // Write result to 0x3C0, which is in data mode.

    width = 80;
    height = 25;
}

void x86TextModeLog::DrawChar(int xpos, int ypos, char c, char fore,
                              char back) {
    int location = ((ypos * width) + xpos) * 2;
    uint8_t attrib = (back << 4) | (fore & 0x0F);
    buffer[location] = c;
    buffer[location + 1] = attrib;
}

void x86TextModeLog::Scroll(int linesDown) {
    if (linesDown < 0) {
        //TODO: Implement
        asm("hlt");
        return;
    }

    linesDown = 1;

    uint32_t screen_size = width * height * 2;
    uint32_t line_down_offset = width * (linesDown)*2;
    uint32_t needed_region_start = baseAddr + line_down_offset;
    uint32_t needed_region_size = screen_size - line_down_offset;
    uint8_t tempBuffer[needed_region_size];

    // Step 1: Copy needed part of screen to buffer
    memcpy(&tempBuffer, (void*)(needed_region_start), needed_region_size);

    // Step 2: Copy buffer to screen.
    memcpy((void*)baseAddr, &tempBuffer, needed_region_size);

    // Step 3: Clear lines at bottom
    memset((void*)(baseAddr + needed_region_size), 0, line_down_offset);

    y -= linesDown;
}

void x86TextModeLog::Clear() {
    x = 0;
    y = 0;
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            DrawChar(x, y, ' ', foreColor, backColor);
        }
    }

    x = left_margin;
}

void x86TextModeLog::Newline() {
    // Fill blank space with attribute...
    for (unsigned int i = x; i < width; i++) {
        DrawChar(i, y, ' ', foreColor, backColor);
    }
    // Reset and scroll
    y++;
    x = left_margin;
    for (unsigned int i = 0; i < left_margin; i++) {
        DrawChar(i, y, ' ', foreColor, backColor);
    }
}

void x86TextModeLog::ChangeForegroundColor(unsigned char c) {
    foreColor = c;
}
void x86TextModeLog::ChangeBackgroundColor(unsigned char c) {
    backColor = c;
}
void x86TextModeLog::Print(char c) {
    if (c == '\n') {
        Newline();
        return;
    }
    if (x >= (signed int)width) {
        Newline();
    }
    DrawChar(x, y, c, foreColor, backColor);

    x++;
}