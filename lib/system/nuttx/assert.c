/*
 * Copyright (c) 2018, Pinecone Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	assert.c
 * @brief	NuttX assertion support.
 */

#include <assert.h>

/**
 * @brief Assertion macro for NuttX-based applications.
 * @param cond Condition to evaluate.
 */
void metal_assert(int cond)
{
	DEBUGASSERT(cond);
}
