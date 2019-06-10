#include <kernel/arch/i386/drivers/bga.h>
#include <kernel/arch/i386/ports.h>
#include <kernel/log.h>
#include <string.h>
BGA x86driver_bga;

inline void BlBochsVbeWrite(uint16_t index, uint16_t value) {
    outw(VBE_DISPI_IOPORT_INDEX, index);
    outw(VBE_DISPI_IOPORT_DATA, value);
}

inline void BGA::ChangeBank(uint16_t bankNo) {
    if (bankNo == currentBank) {
        return;
    }
    currentBank = bankNo;
    BlBochsVbeWrite(VBE_DISPI_INDEX_BANK, bankNo);
}

inline uint16_t BGA::GetBankForPixel(unsigned int x, unsigned int y) {
    uint32_t address = ((screenWidth * y) + x) * bytesPerPixel;
    uint16_t bankNo = address / bytesPerBank;
    return bankNo;
}

inline void BGA::PlotPixel(unsigned int x, unsigned int y, uint32_t color) {
    // Set pixel bank...
    uint16_t pixelBank = GetBankForPixel(x, y);
    if (pixelBank != currentBank) {
        currentBank = pixelBank;
        BlBochsVbeWrite(VBE_DISPI_INDEX_BANK, pixelBank);
    }

    uint32_t address = ((screenWidth * y) + x) * bytesPerPixel;
    address -= currentBank * bytesPerBank;

    buffer[(address >> 2)] = color;
}

void BGA::PlotPixel(unsigned int x, unsigned int y, Color color) {
    // Convert color to 0RGB
    uint32_t c = color.b ^ (color.g << 8) ^ (color.r << 16);
    PlotPixel(x, y, c);
}

void BGA::LineCopy(int lineSrc, int lineDest) {
    // There are four possibilities here. They are ordered from
    // Fastest to slowest

    // 1. Both lines are in the same bank. Simply do a memcpy.
    // 2. The source line is two banks, but the destination
    //    is in one. Copy source to a buffer, then copy buffer
    //    to the destination.
    // 3. The source line is one bank, but the destination
    //    is in two. Copy source to a buffer, then copy part
    //    of the buffer to the destination, switch banks,
    //    and the other part of the buffer to it's destination.
    // 4. Both lines are in two banks. Copy part of source
    //    to a buffer, switch banks, then the other part of source.
    //    Then, copy part of the of the buffer to the dest,
    //    switch banks, and the other part of the buffer to dest

    // First, identify the banks that each lines start and end point will be in.
    // Because of the size of the banks, a line can only be split by 1 bank, so
    // two samples are okay.

    uint16_t src_bank_head = GetBankForPixel(0, lineSrc);
    uint16_t src_bank_tail = GetBankForPixel(screenWidth - 1, lineSrc);
    uint16_t dest_bank_head = GetBankForPixel(0, lineDest);
    uint16_t dest_bank_tail = GetBankForPixel(screenWidth - 1, lineDest);
    bool src_two_banks = src_bank_head != src_bank_tail;
    bool dest_two_banks = dest_bank_head != dest_bank_tail;
    unsigned int lineWidth = screenWidth * bytesPerPixel;

    // No splits...
    if (!src_two_banks && !dest_two_banks) {
        if (src_bank_head == dest_bank_head) {
            // Just memcpy, no bank switching required.
            ChangeBank(src_bank_head);  // Ensure we're in the correct bank...

            memcpy((void*)(GetAbsoluteAddress(lineDest)),
                   (void*)(GetAbsoluteAddress(lineSrc)), lineWidth);

        } else {
            uint8_t buffer[lineWidth];
            // Memcpy to buffer
            ChangeBank(src_bank_head);
            memcpy(&buffer, (void*)(GetAbsoluteAddress(lineSrc)), lineWidth);
            // Switch banks
            ChangeBank(dest_bank_head);
            // Memcpy from buffer
            memcpy((void*)(GetAbsoluteAddress(lineDest)), &buffer, lineWidth);
        }
    } else {
        // 3 possibilities, src is split, dest is split, or both are split...
        // TODO: Finish
    }
}

void BGA::Initialise() {
    BlBochsVbeWrite(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    BlBochsVbeWrite(VBE_DISPI_INDEX_XRES, screenWidth);
    BlBochsVbeWrite(VBE_DISPI_INDEX_YRES, screenHeight);
    BlBochsVbeWrite(VBE_DISPI_INDEX_BPP, VBE_DISPI_BPP_32);
    if (featureFrameBuffer) {
        BlBochsVbeWrite(VBE_DISPI_INDEX_ENABLE,
                        VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);
    } else {
        BlBochsVbeWrite(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED);
    }
    // Force bank to be 0...
    currentBank = 1;
    ChangeBank(0);
}

bool BGA::Supported() {
    // Ask for BGA ID...
    outw(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_ID);
    uint16_t version = inw(VBE_DISPI_IOPORT_DATA);

    if (version >= VBE_DISPI_ID0 && version <= VBE_DISPI_ID5) {
        g_log.Log(LOG_DEBUG, "bga", "Card version 0x%04X detected", version);
        if (version == VBE_DISPI_ID5) {
            g_log.Log(LOG_DEBUG, "bga",
                      "Latest card version, enabling frame buffer");
            featureFrameBuffer = false;
        }
        return true;
    } else {
        g_log.Log(LOG_DEBUG, "bga", "Not found");
    }

    return false;
}