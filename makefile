################################################################################
# The "entry point" for the build process and any other operation which uses   #
# the make commandline tool. 												   #
# 																			   #
# Author: Joshua Buchanan (@jrbuchanan02)									   #
# Created: 2021-xx-xx (I don't know when?)									   #
# Modified: 2022-03-22														   #
# Version: 2																   #
#																		       # 
# This file allows the recursive and reflective building of all source files   #
# within Garbage OS. This process is recursive in that each directory 		   #
# containing source files has a makefile within it which builds that directory #
# and calls makefiles in its subdirectories. It is also reflective in that it  #
# uses any reflection-features present in make to find all necessary files.    #
#																		       #
# The build process:														   #
# - Installation:															   #
#   In the installation phase of the build process, we need to clone and then  #
# build GCC, G++, and GAS for the native host and all supported targets by     #
# GOS. This requires:														   #
# 		+ A list of all supported targets									   #
# 		+ The URL to clone GCC, G++, GAS from								   #
################################################################################

# targets we support
supported_archs := aarch64 arm i386 ia64 x86_64
# GCC typically doesn't like bare metal targets, so we "target" linux
# since GOS has to be written as a freestanding architecture, this is fine.
#
# On EFI targets, we have to use objcopy to transfer the contents of the 
# ELF binary to a PE format. (i.e., turn gos.out into gos.exe but call 
# it gos.efi instead).
target := linux

