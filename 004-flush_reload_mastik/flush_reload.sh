#!/bin/bash


# Build Mastik
cd Mastik
./configure
make
sudo make install
cd ..

# Run Flush-Reload
cd Mastik/demo
./FR-1-file-access
