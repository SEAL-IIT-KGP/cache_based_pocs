#!/bin/bash

# Collect cache hit data

cd ../001-cache_hit/
make all
make run > hits
make run >> hits
make run >> hits
make run >> hits
make run >> hits
make run >> hits
make run >> hits
make run >> hits
make run >> hits
make run >> hits
mv hits ../003-threshold_choice
make clean

# Collect cache miss data
cd ../002-cache_miss
make all
make run > misses
make run >> misses
make run >> misses
make run >> misses
make run >> misses
make run >> misses
make run >> misses
make run >> misses
make run >> misses
make run >> misses
mv misses ../003-threshold_choice
make clean

# Plot
cd ../003-threshold_choice
python3 plot.py
