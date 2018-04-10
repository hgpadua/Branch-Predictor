Huey Padua
EEL4768
README

Branch Predictor Simulator

The branch prediction source code is saved under bpsim.c, which is a g share branch predictor that takes in two parameters, M and N, where M is the number of lower program counter bits, which are used to form the prediction table index and N is the number of bits in the global branch history register. The simulator will read in a trace file that contains a PC address and the outcome, within each line. There are two files provided to fit project specifications, which are “mcf_trace.txt” and “gobmk_trace.txt”. I order for the program to compile and run, the parameters and trace file must be passed in as argvs. After compilation, the program should print the configurations and misprediction rate ratio.

Examples of how to run simulator:
gcc -o sim bpsim.c
./sim 8 5 mcf_trace.txt

This will simulate g share branch predictor with M(number of pc bits) as 8, N(global history register bits) as 5, and “mcf__trace.txt” as the trace file.