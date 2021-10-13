/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicrolecronicsx Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	cache.c
 * @brief	CACHE operation primitives for libmetal.
 */

#include <metal/cache.h>

void metal_cache_flush(void *addr, unsigned int len)
{
	__metal_cache_flush(addr, len);
}

void metal_cache_invalidate(void *addr, unsigned int len)
{
	__metal_cache_invalidate(addr, len);
}
