# src/kernel/loaders
This directory contains the machine-specific entry points for Garbage OS. Some
entry points are unique to a specific combination of architecture and bootloader
while others join several architectures under one bootloader (i.e., EFI should 
do that).