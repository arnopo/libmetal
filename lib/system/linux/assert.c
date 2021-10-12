/*
 * Copyright (c) 2018, Xilinx Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	assert.c
 * @brief	Linux assertion support.
 */

#include <assert.h>

/**
 * @brief Assertion macro for Linux-based applications.
 * @param cond Condition to evaluate.
 */
void metal_assert(int cond)
{
	assert(cond);
}
