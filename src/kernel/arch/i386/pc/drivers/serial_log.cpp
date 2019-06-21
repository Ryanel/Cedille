#include <kernel/arch/i386/drivers/serial_log.h>
#include <kernel/arch/i386/ports.h>
#include <stdint.h>
#include <string.h>

#define PORT 0x3F8  // COM1

void SerialLog::Init() {
    outb(PORT + 1, 0x00);  // Disable all interrupts
    outb(PORT + 3, 0x80);  // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);  // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00);  //                  (hi byte)
    outb(PORT + 3, 0x03);  // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B);  // IRQs enabled, RTS/DSR set

    width = 80;
    height = 25;
    isSerial = true;
}
void SerialLog::SendChar(char c) {
    while ((inb(PORT + 5) & 0x20) == 0) {
    }
    outb(PORT, c);
}

void SerialLog::Clear() {
    SendChar(27);
    SendChar('[');
    SendChar('2');
    SendChar('J');

    SendChar(27);
    SendChar('[');
    SendChar('H');
}

void SerialLog::Newline() {
    // Fill blank space with attribute...
    for (unsigned int i = x; i < width; i++) {
        SendChar(' ');
    }
    // Reset and scroll
    y++;
    x = left_margin;
    for (unsigned int i = 0; i < left_margin; i++) {
        SendChar(' ');
    }
}

void SerialLog::ChangeForegroundColor(unsigned char c) {
    foreColor = c;
}
void SerialLog::ChangeBackgroundColor(unsigned char c) {
    backColor = c;
}
void SerialLog::Print(char c) {
    if (c == '\n') {
        Newline();
        return;
    }
    SendChar(c);
    x++;
}