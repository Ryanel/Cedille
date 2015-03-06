Build Options
====================
Build options are C Preprocessor defines that manipulate the features compiled into the kernel. They can be changed only at compile time.

> Note, each is prefixed by -D in the makefile, kconfig.h or on the command line

* `DEBUG`
Prints / enables all debug code. Do not use for production releases.
* `OPTION_ENABLE_BUILDENV_PRINTING`
Prints the enviroment, such as the architecture and source of the release.