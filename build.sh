
generating_code="Generating Code..."
compiling_code="Compiling Code..."

echo_cmd=echo
make_cmd=make
python_cmd=python3

generator_file=source/build/generation/generator_main.py
make_file=source/build/compilation/makefile

make_options="-j -B -f "$make_file" all"
$make_cmd $make_options