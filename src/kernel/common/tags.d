module kernel.common.tags;

import std.algorithm.mutation : swap;
import core.volatile;

/// memory entry
struct MemoryEntry 
{
    const uint addressHigh; /// high part of address
    const uint addressLow; /// low part of address

    const uint lengthHigh; /// high part of length
    const uint lengthLow; /// low part of length
    const uint type; /// type of the entry
    const uint version_; /// entry version
    const uint reservedData; /// what it says
}

/// Moves around the memory entries in place so that they will work more easily with
/// making neat tables
///
/// Params:
///     sizes pointer to two uints that store first the size of the tag and second the
///         size of an entry. Both of these are important to move through the code. 
///         This pointer is from ecx, per the c calling convention
///     entries the pointer to the address containing the entries. These are sorted 
///         based on their type (1, 3, 4, other). Such sorting makes initializing the
///         page tables easier (when we get to that).
///         This pointer is from edx, per the c calling convention
/// Returns:
///     how many memory entries there are.
extern (C) uint memSort(ref uint* sizes, ref MemoryEntry* entries) @system
{
    uint tagSize = sizes[0];
    tagSize -= 12; // subtract out type, size, and entry size

    immutable uint entrySize = sizes[1];

    immutable uint totalSize = tagSize / entrySize; // get the total length

    for(uint i = 0; i < totalSize; i++) {
        immutable uint type = (*(entries + i * entrySize)).type;

        for (uint j = i; j < totalSize; j++) {
            switch ((*(entries + j * entrySize)).type) {
            case 1: // all of the one's should be first
                if (type != 1) {
                    swap (*(entries + i * entrySize), *(entries + j * entrySize));
                }
                break;
            case 3: // all of the threes should be after the 1's
                if (type != 1 || type != 3) {
                    swap (*(entries + i * entrySize), *(entries + j * entrySize));
                }
                break;
            case 4: // all of the fours should be after 1's and 3's
                if (type != 1 || type != 3 || type != 4) {
                    swap (*(entries + i * entrySize), *(entries + j * entrySize));
                }
            default: // we found something that should not be in front of type
                break;
            }
        }
    }

    return totalSize;
}

