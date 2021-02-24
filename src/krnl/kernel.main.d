module kernel.main;

/// main function, must be marked extern C because that is the linkage NASM 
/// uses.
///
/// Version: SNAPSHOT 2021-02-24
/// History:
///     SNAPSHOT 2021-02-24 - spins for eternity
extern(C) void main()
{
    while (true)
    {
        // spin!!
        // weeee
    }
}