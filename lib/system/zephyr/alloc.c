/*
 * Copyright (c) 2017, Linaro Limited. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	zephyr/alloc.c
 * @brief	Zephyr libmetal memory allocation handling.
 */

#include <kernel.h>
#include <metal/alloc.h>
#include <metal/compiler.h>
#include <stdlib.h>

#if (CONFIG_HEAP_MEM_POOL_SIZE > 0)
void *metal_allocate_memory(unsigned int size)
{
	return k_malloc(size);
}

void metal_free_memory(void *ptr)
{
	k_free(ptr);
}

#else

void *metal_weak metal_zephyr_allocate_memory(unsigned int size)
{
	(void)size;
	return NULL;
}

void metal_weak metal_zephyr_free_memory(void *ptr)
{
	(void)ptr;
}

void *metal_allocate_memory(unsigned int size)
{
	return metal_zephyr_allocate_memory(size);
}

void metal_free_memory(void *ptr)
{
	metal_zephyr_free_memory(ptr);
}
#endif /* CONFIG_HEAP_MEM_POOL_SIZE */
