#!/bin/bash

ZEPHYR_TOOLCHAIN_VARIANT=zephyr
ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk
ZEPHYR_BASE=$TRAVIS_BUILD_DIR/deps/zephyr
ZEPHYR_SDK_VERSION=0.11.2
ZEPHYR_SDK_DOWNLOAD_FOLDER=https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v$ZEPHYR_SDK_VERSION
ZEPHYR_SDK_SETUP_BINARY=zephyr-sdk-$ZEPHYR_SDK_VERSION-setup.run
ZEPHYR_SDK_DOWNLOAD_URL=$ZEPHYR_SDK_DOWNLOAD_FOLDER/$ZEPHYR_SDK_SETUP_BINARY
FREERTOS_ZIP_URL=https://cfhcable.dl.sourceforge.net/project/freertos/FreeRTOS/V10.0.1/FreeRTOSv10.0.1.zip
GCC_ARM_COMPILER_PACKAGE=gcc-arm-embedded_7-2018q2-1~bionic1_amd64.deb
CC=gcc-4.8
CXX=g++-4.8
CMAKE_VERSION=3.15.3


build_linux(){
	echo  "build for linux"
	sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test &&
	sudo apt-get update -qq &&
	sudo apt-get install libsysfs-dev libhugetlbfs-dev make gcc-4.8;

	mkdir -p build-linux &&
	cd build-linux &&
	cmake .. -DWITH_TESTS_EXEC=on &&
	make VERBOSE=1 all test &&
	exit 0
}
main(){
   build_linux
}

main 
