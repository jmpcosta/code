## Purpose

This projects provides an ASCII conversion tool to upper or lower case.
The tool uses Intel CPU vector extensions (SIMD) to speed up the conversion process.

## Build

### Requirements

- GCC
- Intel Intrisics (support of AVX / AVX2 / AVX512)

## Building the tool

cd Release

make

## Usage


aconv [ -a | -A ] < input_file > output_file

Where:

-a      Lower case

-A      Upper Case

## Limitations

AVX512 was not yet tested.




