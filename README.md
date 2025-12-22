<img src="https://github.com/user-attachments/assets/c42b3dd7-87a2-4a59-8faf-c9e422a6131a" width=256><b>
# NEONIX: A Modern Alternative to UNIX-like Operating Systems

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)  

**NEONIX** is an x86_64 Operating System built off of the Naho kernel, which is built in C, and assembly. The Naho kernel aims to mix **performance**, **security**, and **modern hardware support** all into one package.

## Getting Started

### Prerequisites
- A cross compiler
- QEMU
- Fingers (for typing :P)

### Building, and running
Building and running NEONIX is super simple :)
```
make bootloader && make run -j$(nproc --all)
```

## Configuration
To configure the Naho kernel, enter the following command
```
make menuconfig
```

## Contributions
Any contributions to NEONIX is very much appreciated :)
