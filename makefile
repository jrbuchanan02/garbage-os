multiboothdr := $(shell find /src/kernel/bios -name multiboot2header.s)

i386_s_files := $(shell find /src/kernel -name *i386.s)
i386_d_files := $(shell find /src/kernel -name *.d)

long_s_files := $(shell find /src/kernel -name *long.s)
long_starter := $(shell find /src/kernel -name starti386.s)
long_d_files := $(shell find /src/kernel -name *.d)

ddocumentatn := $(shell find /doc)
d_interfaces := $(shell find /intf)

.PHONY: debug-i386

debug-long:
	