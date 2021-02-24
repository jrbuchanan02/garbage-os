module kernel.main;

import core.volatile; // I think that's the term...?

/// main function, must be marked extern C because that is the linkage NASM 
/// uses.
///
/// Version: SNAPSHOT 2021-02-24
/// Params:
///     size = amount of bits available
/// History:
///     SNAPSHOT 2021-02-24 - spins for eternity
extern(C) void main(uint size)
{
    version (x86_64)
    {
        assert(size == 64);
    }
    else version (x64)
    {
        assert(size == 32);
    }
    while (true)
    {
        // spin!!
        // weeee
    }
}