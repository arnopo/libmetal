/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	freertos/alloc.c
 * @brief	FreeRTOS libmetal memory allocattion definitions.
 */

#include "FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif

void *metal_allocate_memory(unsigned int size)
{
	return pvPortMalloc(size);
}

void metal_free_memory(void *ptr)
{
	vPortFree(ptr);
}

#ifdef __cplusplus
}
#endif

