# MiniJV880 ![Github Build Status](https://github.com/giulioz/mini-jv880/actions/workflows/build.yml/badge.svg)

MiniJV880 is a rompler-style synthesizer closely modeled on the famous JV-880 by a well-known Japanese manufacturer running on a bare metal Raspberry Pi (without a Linux kernel or operating system).

## Build instructions
These are based on the [minidexed build instructions](https://github.com/probonopd/MiniDexed/wiki/Development#building-locally).\
Since the build actions are stored as git actions in/.github/workflows/build.yml , these can be exececuted locally using [nektos act](https://github.com/nektos/act).

### Get dependencies
Get specific commits from other repos.
```bash
$ bash submod.sh
```
### Get compilation tolchain
```bash
$ bash get_arm_toolchain.sh
```
<!-- 
### Build for specific RPI model (1-5)

```bash
$ bash build.sh 3 # Pass RPI number
``` -->
### Make the contend for the SD card

```bash
$ bash build_sd.sh 3 # Pass RPI number
```

A



## Acknowledgements

This project stands on the shoulders of giants. Special thanks to:

- [nukeykt](https://github.com/nukeykt) for the [Nuked SC-55](https://github.com/nukeykt/Nuked-SC55) emulator, on which this synth is based on
- [probonopd](https://github.com/probonopd) for [MiniDexed](https://github.com/probonopd/MiniDexed), which served as basis for this version
- [rsta2](https://github.com/rsta2) for [Circle](https://github.com/rsta2/circle), the library to run code on bare metal Raspberry Pi (without a Linux kernel or operating system) and for the bulk of the MiniDexed code
- [smuehlst](https://github.com/smuehlst) for [circle-stdlib](https://github.com/smuehlst/circle-stdlib), a version with Standard C and C++ library support
