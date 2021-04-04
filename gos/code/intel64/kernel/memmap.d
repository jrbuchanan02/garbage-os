module gos.code.intel64.kernel.memmap;

/// swaps the two memory map entries 4 bytes at a time.
void swap(in MemoryMapInfo* memoryMap, MemoryMapEntry* lhs, MemoryMapEntry* rhs) {
    uint value;

    for (uint i = 0; i < (*memoryMap).entrySize / 4; i++) {
        value = (cast(uint*)lhs)[i];
        (cast(uint*)lhs)[i] = (cast(uint*)rhs)[i];
        (cast(uint*)rhs)[i] = value;
    }
}

/**
 * Multiboot 2 information structure as defined by the Multiboot 2 standard
 * upon calling 
 */
extern (C) struct Mb2Info {
    const uint totalSize; /// total size of the structure, including headers, tags, padding, and the terminating field
    /// a reserved value that we will ignore. If we so wanted we could determine if 
    /// we are running on a later version of mb2 than Garbage OS was built for by 
    /// comparing this value with 0 (MB2 bootloaders currently set it to 0).
    const uint reservedValue;   
}

/// format of tag header
extern(C) struct MB2TagHeader {
    const uint type; /// tag type
    const uint size; /// tag size
}

/// mixin template for some common values
mixin template MB2TagStart(uint _type) {
    const uint type = _type;
    const uint size;
}

/// ditto
mixin template MB2TagStart(uint _type, uint _size) {
    const uint type = _type;
    const uint size = _size;
}

/// basic memory information, not a memory map
extern(C) struct BasicMemoryInfo {
    mixin MB2TagStart!(4, 16);
    uint lowerMemory; /// KiB's in lowmem
    uint upperMemory; /// KiB's in hi mem
}

/// boot device information
extern(C) struct BIOSBootDeviceInfo {
    mixin MB2TagStart!(5, 20);
    uint biosDevice;    /// IDK
    uint partition;     /// partition we came from?
    uint subpartition;  /// subpartition we came from?
}

/// boot command line
extern(C) struct BootCommandLineInfo {
    mixin MB2TagStart!(1);
    /// command line itself
    const char[] commandLine;
}

/// modules tag
extern(C) struct ModulesInfo {
    mixin MB2TagStart!(3);

    /// module start
    uint startAddress;

    /// module end
    uint endAddress;

    /// the item for the string - MB2 not clear on what the string means...
    const char* stringLocation;
}

/// ELF-symbols tag
extern (C) struct ELFSymbolsInfo {
    mixin MB2TagStart!(9);

    const ushort entryCount; /// some ELF stuff I don't understand yet
    const ushort entrySize; /// ditto
    const ushort shndx; /// ditto
    const ushort reserved; /// reserved data
    /// immediately after exist the section headers
    /// these are not yet listed here
}

/// Memory Map (what we really want in this file!)
extern(C) struct MemoryMapInfo {
    mixin MB2TagStart!(6);
    const uint entrySize; /// size of an entry
    const uint entryVersion; /// entry version

    // entries
}

/// entry within the memory map
extern(C) struct MemoryMapEntry {
    const ulong baseAddress;    /// base address of the memory
    const ulong length; /// length of the memory
    /// memory type
    /// 1 -> general purpose RAM - mark half as executable, other half as not executable
    /// mark all as globally present, writable, and not system
    /// 2 -> reserved, mark as not present, not writeable, and not executable
    /// 3 -> ACPI reclaimable - mark as system, present, writeable, and not executable.
    /// 4 -> Must be preserved - mark as system, present, writeable, and executable. (we don't necessarily
    /// know what it does)
    /// 5 -> Bad RAM - mark as system, not present, not writeable, and not executable.
    /// 6+-> reserved, mark as not present, not writeable, and not executable.
    const uint type;
    const uint reserved; /// reserved information
}
/// end of tags tag
extern(C) struct EndTag {
    mixin MB2TagStart!(0, 8);
}

/// parses memory map to help paging.s assemble the page tables and initialize
/// all of the RAM in an orderly fashion. There are going to be 5 types of memory:
///
///     1. CRAM (Code RAM), i.e., general purpose RAM that we allow the processor to
/// execute (assuming that we have support for execute disable bit)
///     2. DRAM (Data RAM), i.e., general purpose RAM that we do not allow the processor
/// to execute (assuming that we have support for execute disable bit)
///     3. ACPI, i.e., memory marked as ACPI reclaimable but that we will mark as read only
/// because I need to read up on what is meant by ACPI reclaimable.
///     4. PRES, i.e., memory that we have been asked to preserve when the machine enters a 
/// hibernation state. 
///     5. BKRN, i.e., memory marked as bad or given a type other than 1, 3, or 4. Other than 
/// 5, which specifies bad ram, all values other then 1, 3, and 4 are reserved. We will indicate
/// to the processor that BRKN RAM is not present, but we will still put it into the page table. 
/// The reason for explicitly indicating its absence is because I feel that the bootloader code
/// should be *as pedantic as possible*, and not hesitate to be overly tidy
///
/// Params:
///     multiboot2InformationStructure: value that Multiboot2 leaves in ebx that gives us tags which
///     will help us *now*
extern(C) uint parseMemmap(ref Mb2Info* multiboot2InformationStructure) {
    // iterate through the mb2info structure until finding the memory size

    // have pointer in such a way that we can move it along without care for
    // whether we will do the broken to it.
    void* pointer = cast(void*)multiboot2InformationStructure;

    uint position = 8;

    while (*cast(const uint*)(pointer + position) != 6) {
        if (*cast(const uint*)(pointer +position) == 0) {
            return 0x00F1_00F1; // oof! oof!
        }

        uint fullSize = *cast(const uint*)(pointer + position);
        fullSize += fullSize % 8;

        position += fullSize;

    }


    // ensure that we have the memory map

    if (*cast(const uint*)(pointer + position) != 6) {
        return 0x11E5_11E5; // lies lies
    }

    // now that we know we have the memory map, we have to parse it
    // and do some sorting!

    // we should place viable page-table addresses in the front, the
    // one's with the most page-table-capacity (aligned 0x1000 addresses)
    // should be first

    // yet more importantly, we need the entries strictly in the order of 
    // type 1, type 3, type 4, type 5, type [reserved value].

    MemoryMapInfo* memmap = cast(MemoryMapInfo*) (pointer + position);

    MemoryMapEntry* entries = cast(MemoryMapEntry*) (pointer + position + 16);

    // entry length may not equal the defined entry size (which is why the info 
    // structure specifies the length). So we need to keep track of that separately
    // 
    // note that in D:
    // ```
    // int* foo = [1, 2, 3].ptr;
    //
    // assert(foo[1] == *(foo + 1)); 
    // ```
    // compiles and runs successfully
    
    MemoryMapEntry* getEntry(in uint index) {
        return cast(MemoryMapEntry*)((cast(void*)entries) + (index * (*memmap).entrySize));
    }
    // length = size of entries / entry size
    immutable uint length = ((*memmap).size - 16) / (*memmap).entrySize;

    for (uint i = 0; i < length; i++) {
        immutable uint entryType = (*getEntry(i)).type;

        for (uint j = i; j < length; j++) {
            bool swapped = false;
            switch ((*getEntry(j)).type) {
                case 1:
                    if (entryType != 1) {
                        swap(memmap, getEntry(i), getEntry(j));
                        swapped = true;
                    }
                    else if ((*getEntry(i)).length < (*getEntry(j)).length) {
                        swap(memmap, getEntry(i), getEntry(j));
                        swapped = true;
                    }
                    break;
                case 3:
                    if (entryType != 1 && entryType != 3) {
                        swap(memmap, getEntry(i), getEntry(j));
                        swapped = true;
                    }
                    else if (entryType == 3 && (*getEntry(i)).length < (*getEntry(j)).length) {
                        swap(memmap, getEntry(i), getEntry(j));
                        swapped = true;
                    }
                    break;
                case 4:
                    if (entryType != 1 && entryType != 3 && entryType != 4) {
                        swap(memmap, getEntry(i), getEntry(j));
                        swapped = true;
                    }
                    else if (entryType == 4 && (*getEntry(i)).length < (*getEntry(j)).length) {
                        swap(memmap, getEntry(i), getEntry(j));
                        swapped = true;
                    }
                    break;
                default:
                    // we don't want to put this entry after the other entry
                    // unless if we are larger than it *and* it is not type 1, 3, or 4 memory

                    if ((*getEntry(i)).length < (*getEntry(j)).length) {
                        // less than or equal to 0 should not be entirely necessary, but if the memory
                        // map indicates an unsigned value < 0, it is probably some type we do not know about
                        // yet. :|
                        if (entryType == 2 || entryType >= 5 || entryType <= 0) {
                            swap(memmap, getEntry(i), getEntry(j));
                            swapped = true;
                        }
                    }
                    break;
            }

            if (swapped)
                break;
        }

        // at this point, we are ready to jump back to assembly!
    }
    
    return 0;
}
/// aligns address to 4096 bytes, what x64 requires for a page table
/// entry
///
/// Params:
///     address the address to align
/// Returns:
///     the aligned address
extern(C) ulong alignAddress(inout ulong address) {
    immutable ulong amount = address & 0x0fff;
    return address + (0x1000 - amount);
}

/// points information to memory map
extern(C) void* pointToMemoryMap(ref Mb2Info* multiboot2InformationStructure) {
    // iterate through the mb2info structure until finding the memory size

    // have pointer in such a way that we can move it along without care for
    // whether we will do the broken to it.
    void* pointer = cast(void*)multiboot2InformationStructure;

    uint position = 8;

    while (*cast(const uint*)(pointer + position) != 6) {
        if (*cast(const uint*)(pointer+ position) == 0) {
            return cast(void*)0x00F1_00F1; // oof! oof!
        }

        uint fullSize = *cast(const uint*)(pointer + position);
        fullSize += fullSize % 8;

        position += fullSize;

    }


    // ensure that we have the memory map

    if (*cast(const uint*)(pointer + position) != 6) {
        return cast(void*)0x11E5_11E5; // lies lies
    }

    return pointer;
}