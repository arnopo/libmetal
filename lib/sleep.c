/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics Xilinx Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	sleep.c
 * @brief	Sleep primitives for libmetal.
 */

#include <metal/sleep.h>

int metal_sleep_usec(unsigned int usec)
{
	return __metal_sleep_usec(usec);
}
