/*
 * Copyright (c) 2015, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <limits.h>
#include <metal/errno.h>
#include <metal/io.h>
#include <metal/sys.h>

/**
 * @brief	Close a libmetal shared memory segment.
 * @param[in]	io	I/O region handle.
 */
void metal_io_finish(struct metal_io_region *io)
{
	if (io->ops.close)
		(*io->ops.close)(io);
	memset(io, 0, sizeof(*io));
}

/**
 * @brief	Get size of I/O region.
 *
 * @param[in]	io	I/O region handle.
 * @return	Size of I/O region.
 */
size_t metal_io_region_size(struct metal_io_region *io)
{
	return io->size;
}

/**
 * @brief	Get virtual address for a given offset into the I/O region.
 * @param[in]	io	I/O region handle.
 * @param[in]	offset	Offset into shared memory segment.
 * @return	NULL if offset is out of range, or pointer to offset.
 */
void *metal_io_virt(struct metal_io_region *io, unsigned long offset)
{
	return (io->virt != METAL_BAD_VA && offset < io->size
		? (void *)((uintptr_t)io->virt + offset)
		: NULL);
}

/**
 * @brief	Convert a virtual address to offset within I/O region.
 * @param[in]	io	I/O region handle.
 * @param[in]	virt	Virtual address within segment.
 * @return	METAL_BAD_OFFSET if out of range, or offset.
 */
unsigned long metal_io_virt_to_offset(struct metal_io_region *io, void *virt)
{
	size_t offset = (uintptr_t)virt - (uintptr_t)io->virt;

	return (offset < io->size ? offset : METAL_BAD_OFFSET);
}

/**
 * @brief	Get physical address for a given offset into the I/O region.
 * @param[in]	io	I/O region handle.
 * @param[in]	offset	Offset into shared memory segment.
 * @return	METAL_BAD_PHYS if offset is out of range, or physical address
 *		of offset.
 */
metal_phys_addr_t metal_io_phys(struct metal_io_region *io, unsigned long offset)
{
	if (!io->ops.offset_to_phys) {
		unsigned long page = (io->page_shift >=
				     sizeof(offset) * CHAR_BIT ?
				     0 : offset >> io->page_shift);
		return (io->physmap && offset < io->size
			? io->physmap[page] + (offset & io->page_mask)
			: METAL_BAD_PHYS);
	}

	return io->ops.offset_to_phys(io, offset);
}

/**
 * @brief	Convert a physical address to offset within I/O region.
 * @param[in]	io	I/O region handle.
 * @param[in]	phys	Physical address within segment.
 * @return	METAL_BAD_OFFSET if out of range, or offset.
 */
unsigned long
metal_io_phys_to_offset(struct metal_io_region *io, metal_phys_addr_t phys)
{
	if (!io->ops.phys_to_offset) {
		unsigned long offset =
			(io->page_mask == (metal_phys_addr_t)(-1) ?
			phys - io->physmap[0] :  phys & io->page_mask);
		do {
			if (metal_io_phys(io, offset) == phys)
				return offset;
			offset += io->page_mask + 1;
		} while (offset < io->size);
		return METAL_BAD_OFFSET;
	}

	return (*io->ops.phys_to_offset)(io, phys);
}

/**
 * @brief	Convert a physical address to virtual address.
 * @param[in]	io	Shared memory segment handle.
 * @param[in]	phys	Physical address within segment.
 * @return	NULL if out of range, or corresponding virtual address.
 */
void *metal_io_phys_to_virt(struct metal_io_region *io, metal_phys_addr_t phys)
{
	return metal_io_virt(io, metal_io_phys_to_offset(io, phys));
}

/**
 * @brief	Convert a virtual address to physical address.
 * @param[in]	io	Shared memory segment handle.
 * @param[in]	virt	Virtual address within segment.
 * @return	METAL_BAD_PHYS if out of range, or corresponding
 *		physical address.
 */
metal_phys_addr_t metal_io_virt_to_phys(struct metal_io_region *io, void *virt)
{
	return metal_io_phys(io, metal_io_virt_to_offset(io, virt));
}

/**
 * @brief	Read a value from an I/O region.
 * @param[in]	io	I/O region handle.
 * @param[in]	offset	Offset into I/O region.
 * @param[in]	order	Memory ordering.
 * @param[in]	width	Width in bytes of datatype to read.  This must be 1, 2,
 *			4, or 8, and a compile time constant for this function
 *			to inline cleanly.
 * @return	Value.
 */
uint64_t metal_io_read(struct metal_io_region *io, unsigned long offset,
		       memory_order order, int width)
{
	void *ptr = metal_io_virt(io, offset);

	if (io->ops.read)
		return (*io->ops.read)(io, offset, order, width);
	else if (ptr && sizeof(atomic_uchar) == width)
		return atomic_load_explicit((atomic_uchar *)ptr, order);
	else if (ptr && sizeof(atomic_ushort) == width)
		return atomic_load_explicit((atomic_ushort *)ptr, order);
	else if (ptr && sizeof(atomic_uint) == width)
		return atomic_load_explicit((atomic_uint *)ptr, order);
	else if (ptr && sizeof(atomic_ulong) == width)
		return atomic_load_explicit((atomic_ulong *)ptr, order);
#ifndef NO_ATOMIC_64_SUPPORT
	else if (ptr && sizeof(atomic_ullong) == width)
		return atomic_load_explicit((atomic_ullong *)ptr, order);
#endif
	metal_assert(0);
	return 0; /* quiet compiler */
}

/**
 * @brief	Write a value into an I/O region.
 * @param[in]	io	I/O region handle.
 * @param[in]	offset	Offset into I/O region.
 * @param[in]	value	Value to write.
 * @param[in]	order	Memory ordering.
 * @param[in]	width	Width in bytes of datatype to read.  This must be 1, 2,
 *			4, or 8, and a compile time constant for this function
 *			to inline cleanly.
 */
void metal_io_write(struct metal_io_region *io, unsigned long offset,
		    uint64_t value, memory_order order, int width)
{
	void *ptr = metal_io_virt(io, offset);

	if (io->ops.write)
		(*io->ops.write)(io, offset, value, order, width);
	else if (ptr && sizeof(atomic_uchar) == width)
		atomic_store_explicit((atomic_uchar *)ptr, (unsigned char)value,
				      order);
	else if (ptr && sizeof(atomic_ushort) == width)
		atomic_store_explicit((atomic_ushort *)ptr,
				      (unsigned short)value, order);
	else if (ptr && sizeof(atomic_uint) == width)
		atomic_store_explicit((atomic_uint *)ptr, (unsigned int)value,
				      order);
	else if (ptr && sizeof(atomic_ulong) == width)
		atomic_store_explicit((atomic_ulong *)ptr, (unsigned long)value,
				      order);
#ifndef NO_ATOMIC_64_SUPPORT
	else if (ptr && sizeof(atomic_ullong) == width)
		atomic_store_explicit((atomic_ullong *)ptr,
				      (unsigned long long)value, order);
#endif
	else
		metal_assert(0);
}

void metal_io_init(struct metal_io_region *io, void *virt,
	      const metal_phys_addr_t *physmap, size_t size,
	      unsigned int page_shift, unsigned int mem_flags,
	      const struct metal_io_ops *ops)
{
	const struct metal_io_ops nops = {
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	};

	io->virt = virt;
	io->physmap = physmap;
	io->size = size;
	io->page_shift = page_shift;
	if (page_shift >= sizeof(io->page_mask) * CHAR_BIT)
		/* avoid overflow */
		io->page_mask = -1UL;
	else
		io->page_mask = (1UL << page_shift) - 1UL;
	io->mem_flags = mem_flags;
	io->ops = ops ? *ops : nops;
	metal_sys_io_mem_map(io);
}

int metal_io_block_read(struct metal_io_region *io, unsigned long offset,
	       void *restrict dst, int len)
{
	unsigned char *ptr = metal_io_virt(io, offset);
	unsigned char *dest = dst;
	int retlen;

	if (!ptr)
		return -ERANGE;
	if ((offset + len) > io->size)
		len = io->size - offset;
	retlen = len;
	if (io->ops.block_read) {
		retlen = (*io->ops.block_read)(
			io, offset, dst, memory_order_seq_cst, len);
	} else {
		atomic_thread_fence(memory_order_seq_cst);
		while ( len && (
			((uintptr_t)dest % sizeof(int)) ||
			((uintptr_t)ptr % sizeof(int)))) {
			*(unsigned char *)dest =
				*(const unsigned char *)ptr;
			dest++;
			ptr++;
			len--;
		}
		for (; len >= (int)sizeof(int); dest += sizeof(int),
					ptr += sizeof(int),
					len -= sizeof(int))
			*(unsigned int *)dest = *(const unsigned int *)ptr;
		for (; len != 0; dest++, ptr++, len--)
			*(unsigned char *)dest =
				*(const unsigned char *)ptr;
	}
	return retlen;
}

int metal_io_block_write(struct metal_io_region *io, unsigned long offset,
	       const void *restrict src, int len)
{
	unsigned char *ptr = metal_io_virt(io, offset);
	const unsigned char *source = src;
	int retlen;

	if (!ptr)
		return -ERANGE;
	if ((offset + len) > io->size)
		len = io->size - offset;
	retlen = len;
	if (io->ops.block_write) {
		retlen = (*io->ops.block_write)(
			io, offset, src, memory_order_seq_cst, len);
	} else {
		while ( len && (
			((uintptr_t)ptr % sizeof(int)) ||
			((uintptr_t)source % sizeof(int)))) {
			*(unsigned char *)ptr =
				*(const unsigned char *)source;
			ptr++;
			source++;
			len--;
		}
		for (; len >= (int)sizeof(int); ptr += sizeof(int),
					source += sizeof(int),
					len -= sizeof(int))
			*(unsigned int *)ptr = *(const unsigned int *)source;
		for (; len != 0; ptr++, source++, len--)
			*(unsigned char *)ptr =
				*(const unsigned char *)source;
		atomic_thread_fence(memory_order_seq_cst);
	}
	return retlen;
}

int metal_io_block_set(struct metal_io_region *io, unsigned long offset,
	       unsigned char value, int len)
{
	unsigned char *ptr = metal_io_virt(io, offset);
	int retlen = len;

	if (!ptr)
		return -ERANGE;
	if ((offset + len) > io->size)
		len = io->size - offset;
	retlen = len;
	if (io->ops.block_set) {
		(*io->ops.block_set)(
			io, offset, value, memory_order_seq_cst, len);
	} else {
		unsigned int cint = value;
		unsigned int i;

		for (i = 1; i < sizeof(int); i++)
			cint |= ((unsigned int)value << (CHAR_BIT * i));

		for (; len && ((uintptr_t)ptr % sizeof(int)); ptr++, len--)
			*(unsigned char *)ptr = (unsigned char) value;
		for (; len >= (int)sizeof(int); ptr += sizeof(int),
						len -= sizeof(int))
			*(unsigned int *)ptr = cint;
		for (; len != 0; ptr++, len--)
			*(unsigned char *)ptr = (unsigned char) value;
		atomic_thread_fence(memory_order_seq_cst);
	}
	return retlen;
}

