#pragma once
#include <kernel/display/color.h>
#include <stdint.h>

#define VBE_DISPI_TOTAL_VIDEO_MEMORY_MB 16
#define VBE_DISPI_4BPP_PLANE_SHIFT 22

#define VBE_DISPI_BANK_ADDRESS 0xA0000
#define VBE_DISPI_BANK_SIZE_KB 64

#define VBE_DISPI_MAX_XRES 2560
#define VBE_DISPI_MAX_YRES 1600
#define VBE_DISPI_MAX_BPP 32

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA 0x01CF

#define VBE_DISPI_INDEX_ID 0x0
#define VBE_DISPI_INDEX_XRES 0x1
#define VBE_DISPI_INDEX_YRES 0x2
#define VBE_DISPI_INDEX_BPP 0x3
#define VBE_DISPI_INDEX_ENABLE 0x4
#define VBE_DISPI_INDEX_BANK 0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH 0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 0x7
#define VBE_DISPI_INDEX_X_OFFSET 0x8
#define VBE_DISPI_INDEX_Y_OFFSET 0x9
#define VBE_DISPI_INDEX_VIDEO_MEMORY_64K 0xa

#define VBE_DISPI_ID0 0xB0C0
#define VBE_DISPI_ID1 0xB0C1
#define VBE_DISPI_ID2 0xB0C2
#define VBE_DISPI_ID3 0xB0C3
#define VBE_DISPI_ID4 0xB0C4
#define VBE_DISPI_ID5 0xB0C5

#define VBE_DISPI_BPP_4 0x04
#define VBE_DISPI_BPP_8 0x08
#define VBE_DISPI_BPP_15 0x0F
#define VBE_DISPI_BPP_16 0x10
#define VBE_DISPI_BPP_24 0x18
#define VBE_DISPI_BPP_32 0x20

#define VBE_DISPI_DISABLED 0x00
#define VBE_DISPI_ENABLED 0x01
#define VBE_DISPI_GETCAPS 0x02
#define VBE_DISPI_8BIT_DAC 0x20
#define VBE_DISPI_LFB_ENABLED 0x40
#define VBE_DISPI_NOCLEARMEM 0x80

class BGA {
   private:
    bool featureFrameBuffer = false;

    uint16_t currentBank = 0;
    uint32_t bytesPerBank = 0x10000;
    uint32_t bytesPerPixel = 4;

    uint32_t *buffer = (uint32_t *)0xA0000;

    void ChangeBank(uint16_t bank);
    uint16_t GetBankForPixel(unsigned int x, unsigned int y);
    void PlotPixel(unsigned int x, unsigned int y, uint32_t color);
    uintptr_t GetLineAddress(unsigned int line) {
        return line * screenWidth * bytesPerPixel;
    }
    uintptr_t GetBankBaseAddress() {
        return currentBank * bytesPerBank;
    }

    uintptr_t GetAbsoluteAddress(unsigned int line) {
        uint32_t address = (((screenWidth * line)) * bytesPerPixel) -
                           (currentBank * bytesPerBank);
        return 0xA0000 + address;
    }

   public:
    const unsigned int screenWidth = 800;
    const unsigned int screenHeight = 600;

    void Initialise();
    bool Supported();
    void PlotPixel(unsigned int x, unsigned int y, Color color);
    void LineCopy(int lineSrc, int lineDest);
};
extern BGA x86driver_bga;