#!/bin/bash

set -ex
wget -q https://developer.arm.com/-/media/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf.tar.xz
tar xf *-aarch64-none-elf.tar.xz
# Uncomment for ARM Cortex-M (e.g. RPI < 3)
# wget -q https://developer.arm.com/-/media/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-arm-none-eabi.tar.xz
# tar xf *-arm-none-eabi.tar.xz
mkdir -p kernels