#!/bin/bash

readonly TARGET="$1"

# Known good version for PR testing
ZEPHYR_VERSION=v4.2.0

FREERTOS_ZIP_URL=https://sourceforge.net/projects/freertos/files/FreeRTOS/V10.0.1/FreeRTOSv10.0.1.zip

pre_build(){
	# fix issue related to tzdata install, not needed for 24.04 but kept for 22.04 and earlier
	echo 'Etc/UTC' > /etc/timezone || exit 1
	ln -fs /usr/share/zoneinfo/Etc/UTC /etc/localtime || exit 1

	#Create a new virtual environment
	python3 -m venv ./.venv
	source ./.venv/bin/activate

	# add make and cmake
	# cmake from packages will work for 22.04 and later but use pip3 to get the latest on
	# any distro
	apt update || exit 1
   	apt-get install -y make curl || exit 1
	pip3 install cmake || exit 1

	export PROJECT_ROOT=$PWD
}

build_linux(){
	echo  " Build for linux"
	apt-get install -y libsysfs-dev libhugetlbfs-dev gcc &&
	mkdir -p build-linux &&
	cd build-linux &&
	cmake .. -DWITH_TESTS_EXEC=on &&
	make VERBOSE=1 all test &&
	exit 0
}

build_generic(){
	echo  " Build for generic platform "
	apt-get install -y gcc-arm-none-eabi &&
	mkdir -p build-generic &&
	cd build-generic &&
	cmake .. -DCMAKE_TOOLCHAIN_FILE=template-generic || exit 1
	make VERBOSE=1 || exit 1
	exit 0
}

build_freertos(){
	echo  " Build for freertos OS "
      	apt-get install -y gcc-arm-none-eabi unzip &&
      	wget $FREERTOS_ZIP_URL --dot-style=giga > /dev/null &&
      	unzip FreeRTOSv10.0.1.zip > /dev/null &&
	mkdir -p build-freertos &&
	cd build-freertos && export &&
	cmake .. -DCMAKE_TOOLCHAIN_FILE=template-freertos \
		-DCMAKE_C_FLAGS="-I$PWD/../FreeRTOSv10.0.1/FreeRTOS/Source/include/ \
		-I$PWD/../FreeRTOSv10.0.1/FreeRTOS/Demo/CORTEX_STM32F107_GCC_Rowley \
		-I$PWD/../FreeRTOSv10.0.1/FreeRTOS/Source/portable/GCC/ARM_CM3" || exit 1
	make VERBOSE=1 || exit 1
	exit 0
}

build_zephyr(){
	echo  " Build for Zephyr OS "
	sudo apt install --no-install-recommends git cmake ninja-build gperf \
  		ccache dfu-util device-tree-compiler wget python3-dev python3-venv python3-tk \
  		xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1
	pip3 install west || exit 1

	west init --mr $ZEPHYR_VERSION ./zephyrproject || exit 1
	cd ./zephyrproject || exit 1
	west update -n || exit 1
	west zephyr-export || exit 1
	west packages pip --install || exit 1

	echo  "sdk install"
	SDK_VERSION=$(<$PROJECT_ROOT/zephyrproject/zephyr/SDK_VERSION)
	SDK_PATH=$HOME/"zephyr-sdk-$SDK_VERSION"
	#zephyr-sdk-0.17.2
	cd ./zephyr
	west sdk install -t aarch64-zephyr-elf arm-zephyr-eabi riscv64-zephyr-elf \
		xtensa-dc233c_zephyr-elf
	#workaround as zephyr-sdk-0.17.2/setup.sh is not correctly called on west sdk install
	find / -name zephyr-sdk-*
	ls ../../
	ls $SDK_PATH
	yes | $SDK_PATH/setup.sh

	echo  " Build for Zephyr OS "

	source zephyr-env.sh &&
	cd ../.. &&
	# The prj.conf is mandatory for cmake execution, create a file.
	# The base level of the test framework intentionally uses constructors
	#   so we must define this config item in v3.7.0 or later
	#   HOWEVER, this define will cause cmake to fail for v3.6 or earlier
	echo "CONFIG_STATIC_INIT_GNU=y" >prj.conf &&
	# Add dummy fields in the VERSION file to fix compatibility with Zephyr
	# version.cmake file
 	echo "PATCHLEVEL = 0" >>VERSION &&
 	echo "VERSION_TWEAK = 0" >>VERSION &&
	echo  "###### Build for qemu_cortex_m3 ######" &&
	cmake . -DWITH_ZEPHYR=on -DBOARD=qemu_cortex_m3 -DWITH_TESTS=on -Bbuild-zephyr-m3 || exit 1
	cd build-zephyr-m3  || exit 1
	make VERBOSE=1 || exit 1
	cd .. &&
	echo  "###### Build for qemu_cortex_a53 ######" &&
	cmake . -DWITH_ZEPHYR=on -DBOARD=qemu_cortex_a53 -DWITH_TESTS=on -Bbuild-zephyr-a53 \
		|| exit 1
	cd build-zephyr-a53 &&
	make VERBOSE=1  || exit 1
	cd .. &&
	echo  "###### Build for qemu_xtensa ######" &&
	cmake . -DWITH_ZEPHYR=on -DBOARD=qemu_xtensa -DWITH_TESTS=on -Bbuild-zephyr-xtensa &&
	cd build-zephyr-xtensa &&
	cd .. &&
	echo  "###### Build for qemu_riscv64 ######" &&
	cmake . -DWITH_ZEPHYR=on -DBOARD=qemu_riscv64 -DWITH_TESTS=on -Bbuild-zephyr-rscv64 \
		|| exit 1
	cd build-zephyr-rscv64 &&
	make VERBOSE=1  || exit 1
	cd .. &&
	echo  "###### Build for qemu_riscv32 ######" &&
	cmake . -DWITH_ZEPHYR=on -DBOARD=qemu_riscv32 -DWITH_TESTS=on -Bbuild-zephyr-rscv32 \
		|| exit 1
	cd build-zephyr-rscv32 &&
	make VERBOSE=1  || exit 1
	exit 0
}

main(){
	pre_build;

	if [[ "$TARGET" == "linux" ]]; then
		build_linux
	fi
	if [[ "$TARGET" == "generic" ]]; then
		build_generic
	fi
	if [[ "$TARGET" == "freertos" ]]; then
		build_freertos
	fi
	if [[ "$TARGET" == "zephyr" ]]; then
		build_zephyr
	fi
	if [[ "$TARGET" == "zephyr-latest" ]]; then
		ZEPHYR_VERSION=main
		build_zephyr
	fi
}

main
