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

Regular Goals
--------------------
* Document interfaces for porting (shims)
* Real Memory Detection (currently assumes 16 MB on x86)
* Kernel Debugger

Easy Goals
--------------------
* Make scrolling work with Framebuffer

Stretch Goals
--------------------
* SPARC32/64 Support
* Better ARM Support
* SMP

Bugs
--------------------
* Fix IRQ1 on x86 by either binding ALL IRQ's to a dummy handler or writing a real keyboard driver

Technical Debt
--------------------
* Little to no documentation (in progress...!)
* Uncredited code needs to be removed or credited
* An offical style guide
* Comment existing code

