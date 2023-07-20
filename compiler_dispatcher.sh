# A shell script which makes the command to run the compiler_dispatcher.py script
# friendly for use in a cmake program. Recall that this program will be run in
# the build sub-directory, so we need to use ../
python3 ../compiler_dispatcher.py $@