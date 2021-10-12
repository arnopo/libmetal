/*
 * Copyright (c) 2018, Pinecone Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	nuttx/alloc.c
 * @brief	nuttx libmetal memory allocattion definitions.
 */

#include <nuttx/kmalloc.h>

#ifdef __cplusplus
extern "C" {
#endif

void *metal_allocate_memory(unsigned int size)
{
	return kmm_malloc(size);
}

void metal_free_memory(void *ptr)
{
	kmm_free(ptr);
}

#ifdef __cplusplus
}
#endif

