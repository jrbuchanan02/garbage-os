
generating_code="Generating Code..."
compiling_code="Compiling Code..."

echo_cmd=echo
make_cmd=make
python_cmd=python3
cmake_cmd=cmake

repository_root=./
build_directory=./build

generator_makefile=source/build/compilation/run_generators.mk
generator_recipe=run

make_file_switch=--file=
many_jobs_switch=-j
force_build_all=-B

cmake_source_loc=-S
cmake_build_loc=-B

$echo_cmd $generating_code
$make_cmd $make_file_switch$generator_makefile $generator_recipe $force_build_all $many_jobs_switch
$cmake_cmd $cmake_source_loc $repository_root $cmake_build_loc $build_directory
$echo_cmd $compiling_code
cd $build_directory
$make_cmd $many_jobs_switch