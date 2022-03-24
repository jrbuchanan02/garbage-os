# src/kernel/entry
This folder contains the important source files for booting GOS. There are two
types of entry point for GOS:
1. The machine-specific entry points (these establish a uniform environment)
2. The unified entry point (where we begin writing for all architectures).

Machine specific entry points are grouped by architecture and bootloader and
are not present in this directory.