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

void metal_condition_init(struct metal_condition *cv)
{
	/* TODO: Implement condition variable for FreeRTOS */
	(void)cv;
}

int metal_condition_signal(struct metal_condition *cv)
{
	/* TODO: Implement condition variable for FreeRTOS */
	(void)cv;
	return 0;
}

int metal_condition_broadcast(struct metal_condition *cv)
{
	/* TODO: Implement condition variable for FreeRTOS */
	(void)cv;
	return 0;
}

int metal_condition_wait(struct metal_condition *cv,
			 metal_mutex_t *m)
{
	/* TODO: Implement condition variable for FreeRTOS */
	(void)cv;
	(void)m;
	return 0;
}

