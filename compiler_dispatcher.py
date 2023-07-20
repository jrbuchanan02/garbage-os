"""
A simple python program which uses a commandline switch to redirect all of argv
to a separate compiler. This file intends to get around the limitation in cmake
that the C++ compiler cannot be dynamically changed.

I may later put in a file (or modify this one) if cmake has a similar limitation
on assembly files.
"""
import os,sys

compiler_args = ["PLACEHOLDER NAME"]
architecture_switch = ""
compilers = {
    "native": "g++",
    "ia32": "i686-linux-gnu-g++",
    "8664": "i686-linux-gnu-g++",
    "aa64": "aarch64-linux-gnu-g++",
}

for arg in sys.argv:
    if arg.startswith("-arch="):
        architecture_switch = arg.removeprefix("-arch=")
    elif arg == "../compiler_dispatcher.py":
        continue
    else:
        compiler_args.append(arg)
try:
    compilers[architecture_switch]
except KeyError:
    print("Cannot interpret architecture \"" + architecture_switch + "\"")
    print("Args: ", sys.argv)


compiler_args[0] = compilers[architecture_switch]
os.execvp(compiler_args[0], compiler_args)