<img src="https://github.com/user-attachments/assets/c42b3dd7-87a2-4a59-8faf-c9e422a6131a" width=256><b>

# NEONIX: A Modern Alternative to UNIX-like Operating Systems

[![GitHub license](https://img.shields.io/github/license/zanxz-lol/neonix)](https://github.com/zanxz-lol/neonix/blob/master/LICENSE)
[![GitHub contributors](https://img.shields.io/github/contributors/zanxz-lol/neonix)](https://github.com/zanxz-lol/neonix/graphs/contributors)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/zanxz-lol/neonix)](https://github.com/zanxz-lol/neonix/commits)

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
