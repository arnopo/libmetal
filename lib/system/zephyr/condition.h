/*
 * Copyright (c) 2017, Linaro Limited. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	zephyr/condition.h
 * @brief	Zephyr condition variable primitives for libmetal.
 */

#ifndef __METAL_CONDITION__H__
#error "Include metal/condition.h instead of metal/generic/condition.h"
#endif

#ifndef __METAL_ZEPHYR_CONDITION__H__
#define __METAL_ZEPHYR_CONDITION__H__

#include <metal/atomic.h>
#include <metal/errno.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct metal_condition {
	atomic_uintptr_t mptr; /**< mutex pointer.
				 * The condition variable is attached to
				 * this mutex when it is waiting.
				 * It is also used to check correctness
				 * in case there are multiple waiters.
				 */

	atomic_int v; /**< condition variable value. */
};

/** Static metal condition variable initialization. */
#define METAL_CONDITION_INIT	{ ATOMIC_VAR_INIT(0), ATOMIC_VAR_INIT(0) }

#ifdef __cplusplus
}
#endif

#endif /* __METAL_ZEPHYR_CONDITION__H__ */
