#include <data/font-unscii.h>
#include <kernel/arch/i386/drivers/bga.h>
#include <kernel/arch/i386/drivers/bga_log.h>
#include <stdint.h>

Color log_to_color_mappings[] = {
    Color(0x00, 0x00, 0x00),  // 0x0
    Color(0x00, 0x00, 0xAA),  // 0x1
    Color(0x00, 0xAA, 0x00),  // 0x2
    Color(0x00, 0xAA, 0xAA),  // 0x3
    Color(0xAA, 0x00, 0x00),  // 0x4
    Color(0xAA, 0x00, 0xAA),  // 0x5
    Color(0xAA, 0x55, 0x00),  // 0x6
    Color(0xAA, 0xAA, 0xAA),  // 0x7
    Color(0x55, 0x55, 0x55),  // 0x8
    Color(0x55, 0x55, 0xFF),  // 0x9
    Color(0x55, 0xFF, 0x55),  // 0xA
    Color(0x55, 0xFF, 0xFF),  // 0xB
    Color(0xFF, 0x55, 0x55),  // 0xC
    Color(0xFF, 0x55, 0xFF),  // 0xD
    Color(0xFF, 0xFF, 0x55),  // 0xE
    Color(0xFF, 0xFF, 0xFF),  // 0xF
};

void x86BGALog::Init() {
    width = x86driver_bga.screenWidth / font_width;
    height = x86driver_bga.screenHeight / font_height;
    g_log.Log(LOG_INFO, "log", "BGA logging running at %dx%d (%dx%d chars)", x86driver_bga.screenWidth, x86driver_bga.screenHeight, width, height);
}

void x86BGALog::DrawChar(int xpos, int ypos, char c, unsigned char fore, unsigned char back) {
    Color foreColor = log_to_color_mappings[fore];
    Color backColor = log_to_color_mappings[back];

    // Render blank characters as a Space
    if (c < 32) {
        c = 32;
    }

    // Draw the glyph
    unsigned int glyph_index = c - 32;
    unsigned int glyph_x, glyph_y;
    unsigned int screen_x = xpos * font_width;
    unsigned int screen_y = ypos * font_height;

    for (glyph_y = 0; glyph_y < font_height; glyph_y++) {
        for (glyph_x = 0; glyph_x < font_width; glyph_x++) {
            unsigned char data = font_unscii8_bitmap[(uint8_t)glyph_index][glyph_y];

#ifndef FONT_RENDER_INVERSE
            bool glyph_hit = (data >> (font_width - glyph_x)) & 1;
#else
            bool glyph_hit = (data >> (glyph_x)) & 1;
#endif

            if (glyph_hit) {
                x86driver_bga.PlotPixel(screen_x + glyph_x, screen_y + glyph_y, foreColor);
            } else {
                x86driver_bga.PlotPixel(screen_x + glyph_x, screen_y + glyph_y, backColor);
            }
        }
    }
}

void x86BGALog::Scroll(int linesDown) {
    // We essentially need to copy the whole screen up by font_height pixels
    for (unsigned int i = 8; i < x86driver_bga.screenHeight; i++) {
        x86driver_bga.LineCopy(i, i - 8);
    }
    y -= linesDown;
}

void x86BGALog::Clear() {
    x = 0;
    y = 0;
    for (unsigned int y = 0; y < x86driver_bga.screenHeight; y++) {
        for (unsigned int x = 0; x < x86driver_bga.screenWidth; x++) {
            x86driver_bga.PlotPixel(0, 0, Color(0, 0, 0));
        }
    }
    x = left_margin;
}

void x86BGALog::Newline() {
    // Fill blank space with attribute...
    if (backColor != 0) {  // Optimization: Since the screen is by default black (0), don't overwrite any pixels that are already that color.
        for (unsigned int i = x; i < width; i++) {
            DrawChar(i, y, ' ', foreColor, backColor);
        }
    }
    // Reset and scroll
    y++;
    x = left_margin;
    for (unsigned int i = 0; i < left_margin; i++) {
        DrawChar(i, y, ' ', foreColor, backColor);
    }
}

void x86BGALog::ChangeForegroundColor(unsigned char c) {
    foreColor = c;
}
void x86BGALog::ChangeBackgroundColor(unsigned char c) {
    backColor = c;
}
void x86BGALog::Print(char c) {
    if (c == '\n') {
        Newline();
        return;
    }
    if ((unsigned int)x >= width) {
        Newline();
    }
    DrawChar(x, y, c, foreColor, backColor);

    x++;
}