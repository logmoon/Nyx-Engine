# include "base_memory.h"
# include <assert.h>

// Arena Allocator
uintptr_t align_forward(uintptr_t ptr, size_t align)
{
	uintptr_t p, a, modulo;

	assert(IsPowerOfTwo(align)); // Alignement should always be power of two

	p = ptr;
	a = (uintptr_t)align;
	// Same as (p % a) but faster as 'a' is a power of two
	modulo = p & (a-1);

	if (modulo != 0) {
		// If 'p' address is not aligned, push the address to the
		// next value which is aligned
		p += a - modulo;
	}
	return p;
}

void arena_init(Arena* arena, void* backing_buffer, size_t backing_buffer_length)
{
	MemoryZeroStruct(arena);
	arena->buf = (u8*)backing_buffer;
	arena->buf_len = backing_buffer_length;
}
void* arena_alloc(Arena* arena, size_t size)
{
	void* result = 0;
	// Align 'curr_offset' forward to the specified alignment
	uintptr_t curr_ptr = (uintptr_t)arena->buf + (uintptr_t)arena->curr_offset;
	uintptr_t offset = align_forward(curr_ptr, DEFAULT_ALIGNMENT);
	offset -= (uintptr_t)arena->buf; // Change to relative offset

	// Check to see if the backing memory has space left
	if (offset+size <= arena->buf_len) {
		void *ptr = &arena->buf[offset];
		arena->prev_offset = offset;
		arena->curr_offset = offset+size;

		// Zero new memory by default
		MemoryZero(ptr, size);
		result = ptr;
	}

	return result;
}
void arena_clear(Arena* arena)
{
	MemoryZero(arena->buf, arena->buf_len);
	arena->curr_offset = 0;
	arena->prev_offset = 0;
}
