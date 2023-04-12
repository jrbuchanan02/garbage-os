"""
Generates an array of 256 CRC-32s for each polynomial test
"""
import os
from typing import ByteString, List

from crc import Calculator, Crc32, Configuration, Byte


uefi_configuration = Configuration(width=32, 
                                   polynomial=0x04C11DB7, 
                                   init_value=0xFFFFFFFF, 
                                   final_xor_value=0xFFFFffff, 
                                   reverse_input=True, 
                                   reverse_output=True)
uefi_crc32 = Calculator(uefi_configuration, True)
ieee_crc32 = Calculator(Crc32.CRC32, True)

def un_iterable(iterable_thing):
    as_list = []
    while True:
        try:
            as_list.append(next(iterable_thing))
        except StopIteration:
            return as_list

values:List[str] = []
uefi_crcs:List[int] = []
ieee_crcs:List[int] = []

def add_single_value(value:str):
    values.append(value)
    uefi_crcs.append(uefi_crc32.checksum(bytes(values[i], "ascii" )))
    ieee_crcs.append(ieee_crc32.checksum(bytes(values[i], "ascii")))

for i in range(4096):
    add_single_value(str(i))

add_single_value("123456789")



def header_scope_guard(file: str) -> str:
    without_slashes = file.replace("/", "_")
    without_spaces = without_slashes.replace(" ", "_")
    without_dashes = without_spaces.replace("-", "_")
    without_parens = without_dashes.replace("(" ,"_")
    without_parens = without_parens.replace(")", "_")
    without_dots = without_parens.replace(".", "_")
    without_plus = without_dots.replace("+", "P")
    return "AUTOGEN_" + without_plus.capitalize()

def write_scope_guard(file, name):
    file.write("// this is an auto-generated file. Edits will not be preserved.\n")
    file.write("#ifndef " + header_scope_guard(name) + "\n")
    file.write("#define " + header_scope_guard(name) + "\n")
    file.write("#ifndef __cplusplus\n")
    file.write("#error \"This file is a C++ include.\"\n")
    file.write("#endif // ifndef __cplusplus\n")

def write_list(file, list_name, count, values, type = "std::uint32_t", in_quotes = False):
    file.write("static " + type + " " + list_name + "[] = {\n")
    for i in values:
        format_string = "\t{},\n"
        if in_quotes:
            format_string=  "\t\"{}\",\n"
        file.write(format_string.format(i))
    file.write("};\n")

def make_file(calc: Calculator, filename: str):
    try:
        os.makedirs(os.path.dirname(filename))
    except FileExistsError:
        pass
    try:
        file = open(filename, "x")
    except FileExistsError:
        file = open(filename, "w")
    write_scope_guard(file, filename)
    file.write("#include <cstdint>\n")
    file.write("#include <cstddef>\n")
    file.write("#include <string>\n")
    file.write("constexpr static std::size_t value_count = " + str(len(values)) + ";\n")
    write_list(file, "values", 256, values, "std::string", True)
    write_list(file, "ieee_crc32", 256, ieee_crcs)
    write_list(file, "uefi_crc32", 256, uefi_crcs)
    file.write("#endif // ifndef " + header_scope_guard(filename) + "\n")
    file.close()
    return

def generate():
    make_file(uefi_crc32, "build/generated/crc32results.h++")

print("running crc32_test.py")
generate()