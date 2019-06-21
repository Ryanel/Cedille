#include <kernel/arch/arm/integrator-cp/drivers/serial_log.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SERIAL_BASE 0x16000000
#define SERIAL_FLAG_REGISTER 0x16000018
#define SERIAL_BUFFER_FULL (1 << 5)

void SerialLogger::Init() {
    width = 80;
    height = 25;
    isSerial = true;
}
void SerialLogger::SendChar(char c) {
    /* Wait until the serial buffer is empty */
    while (*(volatile unsigned long*)(SERIAL_FLAG_REGISTER) & (SERIAL_BUFFER_FULL)) {
    }

    /* Put our character, c, into the serial buffer */
    *(volatile unsigned long*)SERIAL_BASE = c;

    /* Print a carriage return if this is a newline, as the cursor's x position will not reset to 0*/
    if (c == '\n') {
        SendChar('\r');
    }
}

void SerialLogger::SendString(const char* s) {
    size_t i = 0;
    while (s[i] != 0) {
        SendChar(s[i]);
        i++;
    }
}

unsigned char SerialLogger::AnsiEscapeColor(unsigned char termColor, bool background) {
    // HACK: Cooincidentally converts to ansi.
    unsigned char ansiEscape = 30 + termColor;
    if (termColor > 7) {
        termColor = 90 + (termColor - 7);
    }

    if (background) {
        ansiEscape += 10;
    }

    return ansiEscape;
}

unsigned char SerialLogger::ConvertTermTo256(unsigned char termColor) {
    const unsigned char conversion_mappings[] = {
        0,   // 0x0
        4,   // 0x1
        2,   // 0x2
        6,   // 0x3
        1,   // 0x4
        5,   // 0x5
        3,   // 0x6
        7,   // 0x7
        8,   // 0x8
        12,  // 0x9
        10,  // 0xA
        14,  // 0xB
        9,   // 0xC
        13,  // 0xD
        11,  // 0xE
        15,  // 0xF
    };

    if (termColor <= sizeof(conversion_mappings)) {
        return conversion_mappings[termColor];
    }

    return termColor;
}

void SerialLogger::Clear() {
    SendChar(27);
    SendString("[2J");

    SendChar(27);
    SendString("[H");
}

void SerialLogger::Newline() {
    // Fill blank space with attribute...
    for (unsigned int i = x; i < width; i++) {
        SendChar(' ');
    }

    // Send to serial
    SendChar(27);  // Down one line
    SendString("[1B");
    SendChar('\r');  // Move cursor back to x = 0

    // Reset and
    y++;
    x = left_margin;

    for (unsigned int i = 0; i < left_margin; i++) {
        SendChar(' ');
    }
}

void SerialLogger::ChangeForegroundColor(unsigned char c) {
    foreColor = c;

    // Send Color to Terminal
    SendChar(27);
    printf("[38;5;%dm", ConvertTermTo256(c));
}
void SerialLogger::ChangeBackgroundColor(unsigned char c) {
    backColor = c;
    /* 
    FIXME: Broken. Sometimes get an extra space at the end of some characters.
    // Send Color to Terminal
    SendChar(27);
    printf("[48;5;%dm", ConvertTermTo256(c));
    */
}
void SerialLogger::Print(char c) {
    if (c == '\n' && x < width) {
        Newline();
        return;
    } else if (x >= width) {
        Newline();
    }
    SendChar(c);
    x++;
}

void SerialLogger::Scroll() {
}