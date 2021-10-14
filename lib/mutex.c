/*
 * Copyright (c) 2015, Xilinx Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, ST Microelectronics Inc.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	mutex.c
 * @brief	Mutex primitives for libmetal.
 */

#include <metal/mutex.h>

void metal_mutex_init(metal_mutex_t *mutex)
{
	__metal_mutex_init(mutex);
}

void metal_mutex_deinit(metal_mutex_t *mutex)
{
	__metal_mutex_deinit(mutex);
}

int metal_mutex_try_acquire(metal_mutex_t *mutex)
{
	return __metal_mutex_try_acquire(mutex);
}

void metal_mutex_acquire(metal_mutex_t *mutex)
{
	__metal_mutex_acquire(mutex);
}

void metal_mutex_release(metal_mutex_t *mutex)
{
	__metal_mutex_release(mutex);
}

int metal_mutex_is_acquired(metal_mutex_t *mutex)
{
	return __metal_mutex_is_acquired(mutex);
}
