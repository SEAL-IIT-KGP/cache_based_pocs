# Measuring cache hits

A simple program to time cache hits. Strategy:

1. Allocate a reasonably sized array that can fit into the L1d cache of your system

2. Access the array multiple times to ensure its residence in L1d

3. Time its access through userspace `rdtsc`


## Build and run

1. Use `make all` to build the binary

2. `make run` to run. Alternatively, `make run > log` to save the results into a file for analysis.
