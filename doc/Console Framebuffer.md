Text Console Framebuffer
===================
The text console frame buffer is a buffer that stores all the text printed by the system log (the kernel's logging functions). Most of the kernels time during boot was writing data to the screen, not actually doing anything useful like initializing data structures. This is because 

1. Hardware access is slow and uses the system bus
2. It blocks the kernel main thread, as it is the only thread
3. Does this PER CHARACTER PRINTED.

The solution was to avoid hardware accesses until necessary. The framebuffer is used like the old hardware framebuffer was used, but blits every `100 ticks` (`100ms` by default) and all at once to the screen. This prevents IO blocking, and allows the software to boot in  `9ms` (best case) since the system clock was initialized.

It refreshes every `100ms` because user interaction does not require high frame rates on a text mode console. It can be flushed / written to independent of the timer, and should be triggered before critical sections, so the user has all data on screen in the event of a crash (in a function like `kernel_doperiodic()`

Features
--------------------
* Raw terminal buffer of any size and number of screens (within reason)
* A copy on update approach that limits the amount of data transferred.
* Updates only when data has been changed (flagged write).
* Stores as much data as the system needs (and is decoded on write, if needed)
* Can be plain text or rich data (Data with color attributes on x86 VGA text-mode)

Problems
--------------------
* Needs locks to control access to framebuffer while flushing.

API
--------------------
* `void text_console_fb_flush()` Flushes the frame-buffer to console, in a platform specific manner.
* `void text_console_fb_addchar(char c, uint8_t x, uint8_t y)` Writes the character
