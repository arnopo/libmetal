/*
 * Copyright (c) 2015, Xilinx Inc. and Contributors. All rights reserved.
 * Copyright (c) 2021, STMicroelectronics Xilinx Inc. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	utilities.c
 * @brief	Utility routines for libmetal.
 */

#include <metal/utilities.h>


void metal_bitmap_set_bit(unsigned long *bitmap, int bit)
{
	bitmap[bit / METAL_BITS_PER_ULONG] |=
		metal_bit(bit & (METAL_BITS_PER_ULONG - 1));
}

int metal_bitmap_is_bit_set(unsigned long *bitmap, int bit)
{
	return ((bitmap[bit / METAL_BITS_PER_ULONG] &
		metal_bit(bit & (METAL_BITS_PER_ULONG - 1))) == 0) ? 0 : 1;
}

void metal_bitmap_clear_bit(unsigned long *bitmap, int bit)
{
	bitmap[bit / METAL_BITS_PER_ULONG] &=
		~metal_bit(bit & (METAL_BITS_PER_ULONG - 1));
}

int metal_bitmap_is_bit_clear(unsigned long *bitmap, int bit)
{
	return !metal_bitmap_is_bit_set(bitmap, bit);
}

unsigned int
metal_bitmap_next_set_bit(unsigned long *bitmap, unsigned int start, unsigned int max)
{
	unsigned int bit;

	for (bit = start;
	     bit < max && !metal_bitmap_is_bit_set(bitmap, bit);
	     bit++)
		;
	return bit;
}

unsigned int
metal_bitmap_next_clear_bit(unsigned long *bitmap, unsigned int start, unsigned int max)
{
	unsigned int bit;

	for (bit = start;
	     bit < max && !metal_bitmap_is_bit_clear(bitmap, bit);
	     bit++)
		;
	return bit;
}

unsigned long metal_log2(unsigned long in)
{
	unsigned long result;

	metal_assert((in & (in - 1)) == 0);

	for (result = 0; (1UL << result) < in; result++)
		;
	return result;
}
