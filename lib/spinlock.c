/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics Xilinx Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	spinlock.c
 * @brief	Spinlock primitives for libmetal.
 */

#include <metal/spinlock.h>

void metal_spinlock_init(struct metal_spinlock *slock)
{
	atomic_flag_clear(&slock->v);
}

void metal_spinlock_acquire(struct metal_spinlock *slock)
{
	while (atomic_flag_test_and_set(&slock->v)) {
		metal_cpu_yield();
	}
}

void metal_spinlock_release(struct metal_spinlock *slock)
{
	atomic_flag_clear(&slock->v);
}
