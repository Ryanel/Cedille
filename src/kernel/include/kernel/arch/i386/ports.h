#pragma once
#include <stdint.h>

/// Outputs a byte to port
void outb(uint16_t port, uint8_t value);
/// Outputs a word to port
void outw(uint16_t port, uint16_t value);
/// Outputs a long to port
void outl(uint16_t port, uint32_t value);
/// Inputs a byte from port
uint8_t inb(uint16_t port);
/// Inputs a word from port
uint16_t inw(uint16_t port);
