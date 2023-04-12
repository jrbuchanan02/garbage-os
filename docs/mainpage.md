# Garbage OS Wiki

Welcome! This wiki describes the functionality of Garbage OS and the information
necessary to understand its inner workings and develop for the platform.

## Pages

Here you can find a list of pages in the wiki.

[The Boot Process](boot_process.md)
[Multitasking and how Garbage OS Handles it](multitasking.md)


## System Architecture

Garbage OS, like most operating systems, follows a layered architecture. At the
level closest to the hardware is the kernel layer, built on top of the kernel is
the API, followed by the unification layer, and finally the application layer.
Each layer can only communicate with the one below it, but there is a caveat 
that the unification layer appears fully transparent, that is, applications talk
to the unification layer exactly how they would talk to the API.

Each layer is itself split into modules. In the API layer, each module is split
into sub-layers (which get progressively further from the kernel). Each module
is in-fact its own program. 