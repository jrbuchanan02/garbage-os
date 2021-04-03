module gos.intel64.code.kernel.kmain;

import core.volatile;

import std.algorithm.mutation : swap;

///
extern(C) void initRam(ref MemoryMap*, ref MemoryMapEntry*);

/// 
extern (C) void kmain(ref void* mbiStructure) nothrow @system {
    

    ulong offset = 8; // point to the first tag

    while (cast(uint)*(mbiStructure + offset) != 6) { // while we are not looking at a type of 6
        offset += cast(uint)*(mbiStructure + offset + 4);
    }

    MemoryMap* map = cast(MemoryMap)*(mbiStructure + offset);    // get a memory map

    MemoryMapEntry* entries = cast(MemoryMapEntry*)(mbiStructure + offset + 16); // get the pointer to the entries

    immutable uint length = *(map.entrySize) / *(map.size - 16);

    MemoryMapEntry* get(in uint index) {
        if (index >= length) {
            return null;
        }
        else {
            return cast(MemoryMapEntry*)((cast(void*)entries) + (*map).entrySize * index);
        }
    }
    // sort-of insertion sort on memory map
    for (uint i = 0; i < index; i++) {
        immutable uint type = (*get(i)).type; // get the type of the first item

        for (uint j = i + 1; j < index; j++) {  // then, find all of the other items
            bool swapped = false;
            switch (*(get(j)).type) {
            case 1: // general purpose RAM
                swap(get(i), get(j));
                swapped = true;
                break;
            case 3: // acpi information
                if (type != 1) {
                    swap(get(i), get(j));
                    swapped = true;
                }
                break;
            case 4: // values to preserve across hibernation states
                if (type != 1 && type != 3) {
                    swap(get(i), get(j));
                    swapped = true;
                }
                break;
            default: // 5 and reserved values
                if (type != 1 && type != 3 && type != 4) {
                    swap(get(i), get(j));
                    swapped = true;
                }
            }
            // if we swapped an item, break
            if (swapped) {
                break;
            }
        }
    }

    initRam(map, entries);
}

private struct MemoryMap {
    const uint type = 6;
    const uint size;
    const uint entrySize;
    const uint entryVersion; // not necessarily equal to 0
}

private struct MemoryMapEntry { // not guaranteed to keep this size
    const ulong baseAddress;
    const ulong length;
    const uint type;
    const uint reserved; 
}