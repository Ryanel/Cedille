# The Cédlle Hybrid kernel

The Cédille Kernel is a kernel in development. It's goal is to be a complete toy kernel
that can self host.

## Building

Note: You *must* use a Cross Compiler.

1. Ensure your cross compiler is in PATH.
2. Set HOST to your specific architecture, or leave it as default for the default x86 architecture.
3. Call build.sh; the kernel should compile.

You can prefix any command with HOST="target-triple" to target that host.

Now that you have a kernel built:

* To generate an iso image, call iso.sh
* To run the kernel in qemu, call qemu.sh
* To reset everything, run clean.sh

To configure the kernel build process (such as flags),
modify `scripts/config.sh`

### Finding your host value

The value of HOST is the first two parts of the target triple for the platform.

For example, for i686-elf-gcc, your HOST is i686-elf.