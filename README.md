This version of Proteum/IM is a through review of the codebase, converting it
to modern C and using current tools for building and testing. It is ***not***
currently usable. You must use Proteum/IM 2.0.1, as tagged and released in
this project.


# Building requirements
* cmake
* cmocka


# Compilation instructions
```
cd build
cmake ..
make
```

# How to test Proteum/IM
```
ctest
```

# How to test other applications using Proteum/IM

Proteum requires GCC to be installed. To use its GUI it is also necessary 
TCL/TK.

Suposing the executable files are in directory /usr/bin/proteum, run

```
export PROTEUMIMHOME=/usr/bin/proteum
export PATH=$PROTEUMIMHOME:$PATH
proteumim
```

Proteum can also be run using command line. We will consider a source file
named 'cal.c' and '/tmp/cal'  as directory for files for the software under
testing (*** caution! instrunctions below are under review and may now work! ***)
1. Create project: ```pteste -create -S cal -E cal -D /tmp/cal -C "gcc -O -W cal.c -o cal" -research Cal```
1. List interfaces that can be tested (for interface mutation): ```li -D /tmp/cal -P __cal cal  __cal```
1. List functions that can be tested (for mutation analysis): ```li -l -D /tmp/cal __cal __cal```
1. Create test set: ```tcase -create -D /tmp/cal Cal```
1. Add test cases: ```tcase -add -D /tmp/cal -E cal -p "input data" -EE __cal_inst -trace cal```
1. Create mutants: ```muta -create -D /tmp/cal Cal```
1. Run mutants:
11. ```tcase -e  -D /tmp/cal Cal```
11. ```exemuta -exec -D /tmp/cal Cal```
1. Show results: ```report -tcase  -D /tmp/cal -L 512 Cal```


# Further documentation
* [User's Guide](doc/manual-proteumim.pdf)
* [User's Guide (another one)](doc/manual-1.1C.pdf) 
