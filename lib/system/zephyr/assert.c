/*
 * Copyright (c) 2018, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	assert.c
 * @brief	Zephyr assertion support.
 */

#include <zephyr.h>

/**
 * @brief Assertion macro for Zephyr-based applications.
 * @param cond Condition to evaluate.
 */
void metal_assert(int cond)
{
	__ASSERT_NO_MSG(cond);
}
