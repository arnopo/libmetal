/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	spinlock.h
 * @brief	Spinlock primitives for libmetal.
 */

#ifndef __METAL_SPINLOCK__H__
#define __METAL_SPINLOCK__H__

#include <metal/atomic.h>
#include <metal/cpu.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup spinlock Spinlock Interfaces
 *  @{
 */

struct metal_spinlock {
	atomic_flag v;
};

/** Static metal spinlock initialization. */
#define METAL_SPINLOCK_INIT		{ATOMIC_FLAG_INIT}

/**
 * @brief	Initialize a libmetal spinlock.
 * @param[in]	slock	Spinlock to initialize.
 */
void metal_spinlock_init(struct metal_spinlock *slock);

/**
 * @brief	Acquire a spinlock.
 * @param[in]	slock   Spinlock to acquire.
 * @see metal_spinlock_release
 */
void metal_spinlock_acquire(struct metal_spinlock *slock);

/**
 * @brief	Release a previously acquired spinlock.
 * @param[in]	slock	Spinlock to release.
 * @see metal_spinlock_acquire
 */
void metal_spinlock_release(struct metal_spinlock *slock);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __METAL_SPINLOCK__H__ */
