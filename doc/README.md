List of Development Tasks
===================
This is a incomplete list of things that are required to be completed until the kernel reaches maturity.

Priority Goals
--------------------
* Virtual memory manager (define subsystem, then implement)
* SLAB Heap
* System Calls
* Usermode
* Processes + Scheduler
* Retest all code
* Move all prototypes to their respective headers.

Regular Goals
--------------------
* Physical Memory Manager doesn't understand sections. Either set those frames as used, or scrap it
* Document interfaces for porting (shims)
* Document interfaces for subsystems.
* Rewrite PMM to use a consistent interface for storing used frames - even supporting multiple interfaces. (Stack, commit to bitmap)
* Kernel Debugger

Easy Goals
--------------------
* Move configuration variables to their own file, or cedille/kconfig.h

Stretch Goals
--------------------
* SPARC32/64 Support
* Better ARM Support
* SMP

Bugs
--------------------
* Fix IRQ1 on x86 by either binding ALL IRQ's to a dummy handler or writing a real keyboard driver
* IRQ code may corrupt the stack if optimizations are enabled. Check if its passing a pointer or a register image.

Technical Debt
--------------------
* Little to no documentation (in progress...!)
* Uncredited code needs to be removed or credited
* An official style guide
* Comment existing code

