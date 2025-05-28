#!/bin/bash

#

if [[ $# -ne 1 ]] || ! [[ $1 =~ ^[2-5]$ ]]; then
    echo "Usage: $0 <RPI_VERSION> (allowed values: 2, 3, 4, 5)"
    exit 1
fi

export RPI=$1

if [ -z "${RPI}" ] ; then
  echo "\$RPI missing, exiting"
  exit 1
fi

# Build dependencies and MiniJV880
./build.sh $RPI
# cd src
# make -j
# cd ..
cp ./src/kernel*.img ./kernels/

#Get Raspberry Pi boot files
cd ./circle-stdlib/libs/circle/boot
make
if [ "${RPI}" -gt 2 ]
then
	make armstub64
fi
cd -

# Make zip that contains Raspberry Pi 4 boot files. The contents can be copied to a FAT32 formatted partition on a microSD card
mkdir -p sdcard
cp -r ./circle-stdlib/libs/circle/boot/* sdcard
rm -rf sdcard/config*.txt sdcard/README sdcard/Makefile sdcard/armstub sdcard/COPYING.linux
cp ./src/config.txt ./src/minijv880.ini ./src/*img sdcard/
echo "usbspeed=full" > sdcard/cmdline.txt
cd sdcard
cp ../kernels/* . || true
zip -r ../MiniJV880_$GITHUB_RUN_NUMBER_$(date +%Y-%m-%d).zip *
cd -
