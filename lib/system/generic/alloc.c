/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/alloc.c
 * @brief	generic libmetal memory allocattion definitions.
 */

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void *metal_allocate_memory(unsigned int size)
{
	return malloc(size);
}

void metal_free_memory(void *ptr)
{
	free(ptr);
}

#ifdef __cplusplus
}
#endif
