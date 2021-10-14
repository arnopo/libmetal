/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/condition.c
 * @brief	Generic libmetal condition variable handling.
 */

#include <metal/condition.h>

inline void metal_condition_init(struct metal_condition *cv)
{
	atomic_init(&cv->mptr, 0);
	atomic_init(&cv->waiters, 0);
	atomic_init(&cv->wakeups, 0);
}

inline int metal_condition_signal(struct metal_condition *cv)
{
	if (!cv)
		return -EINVAL;

	atomic_fetch_add(&cv->wakeups, 1);
	if (atomic_load(&cv->waiters) > 0)
		syscall(SYS_futex, &cv->wakeups, FUTEX_WAKE, 1, NULL, NULL, 0);
	return 0;
}

inline int metal_condition_broadcast(struct metal_condition *cv)
{
	if (!cv)
		return -EINVAL;

	atomic_fetch_add(&cv->wakeups, 1);
	if (atomic_load(&cv->waiters) > 0)
		syscall(SYS_futex, &cv->wakeups, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
	return 0;
}

int metal_condition_wait(struct metal_condition *cv,
				       metal_mutex_t *m)
{
	uintptr_t tmpmptr = 0, mptr = (uintptr_t)m;
	int v = 0;

	/* Check if the mutex has been acquired */
	if (!cv || !m || !metal_mutex_is_acquired(m))
		return -EINVAL;

	if (!atomic_compare_exchange_strong(&cv->mptr, &tmpmptr, mptr)) {
		if (tmpmptr != mptr)
			return -EINVAL;
	}

	v = atomic_load(&cv->wakeups);
	atomic_fetch_add(&cv->waiters, 1);

	/* Release the mutex before sleeping. */
	metal_mutex_release(m);
	syscall(SYS_futex, &cv->wakeups, FUTEX_WAIT, v, NULL, NULL, 0);
	atomic_fetch_sub(&cv->waiters, 1);
	/* Acquire the mutex after it's waken up. */
	metal_mutex_acquire(m);

	return 0;
}
