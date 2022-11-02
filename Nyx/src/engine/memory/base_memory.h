# include "../base.h"

//////////////////////
// Credits:
//		https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
//
// Arena Allocator

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

typedef struct arena
{
	u8 *buf;
	size_t buf_len;
	size_t prev_offset;
	size_t curr_offset;

}Arena;

void arena_init(Arena* arena, void* backing_buffer, size_t backing_buffer_length);
void* arena_alloc(Arena* arena, size_t size);
void arena_clear(Arena* arena);
//////////////////////
