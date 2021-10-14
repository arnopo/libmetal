/*
 * Copyright (c) 2017, Linaro Limited. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	zephyr/condition.c
 * @brief	Zephyr libmetal condition variable handling.
 */

#include <metal/condition.h>
#include <metal/irq.h>

extern void metal_generic_default_poll(void);

void metal_condition_init(struct metal_condition *cv)
{
	atomic_init(&cv->mptr, 0);
	atomic_init(&cv->v, 0);
}

int metal_condition_signal(struct metal_condition *cv)
{
	if (!cv)
		return -EINVAL;

	/** wake up waiters if there are any. */
	atomic_fetch_add(&cv->v, 1);
	return 0;
}

int metal_condition_broadcast(struct metal_condition *cv)
{
	return metal_condition_signal(cv);
}

int metal_condition_wait(struct metal_condition *cv,
			 metal_mutex_t *m)
{
	uintptr_t tmpmptr = 0, mptr = (uintptr_t)m;
	int v;
	unsigned int flags;

	/* Check if the mutex has been acquired */
	if (!cv || !m || !metal_mutex_is_acquired(m)) {
		return -EINVAL;
	}

	if (!atomic_compare_exchange_strong(&cv->mptr, &tmpmptr, mptr)) {
		if (tmpmptr != mptr) {
			return -EINVAL;
		}
	}

	v = atomic_load(&cv->v);

	/* Release the mutex first. */
	metal_mutex_release(m);
	do {
		flags = metal_irq_save_disable();
		if (atomic_load(&cv->v) != v) {
			metal_irq_restore_enable(flags);
			break;
		}
		metal_generic_default_poll();
		metal_irq_restore_enable(flags);
	} while (1);
	/* Acquire the mutex again. */
	metal_mutex_acquire(m);
	return 0;
}
