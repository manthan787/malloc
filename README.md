# Malloc
A thread-safe memory allocator based on Buddy Memory Allocation Alogirithm

### Design Overview

The functions provided with this implementation include:

```
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
struct mallinfo mallinfo();
void malloc_stats();
```


Functions `malloc()`, `mallinfo()`, `malloc_stats()` and data structures required by these functions are declared in `malloc.h` and defined in `malloc.c`. `malloc.h` also houses `blocks` which is an array of `Block *` (read block.h for more). `blocks` is responsible for maintaining the free blocks. Per thread arenas are maintained by giving individual copies of `blocks` to each thread. The memory required to create free nodes is obtained using `sbrk` and the starting address of that is stored in `heap`.

Functions `free()`, `realloc()` and `calloc()` can be found respectively in `free.c`, `realloc.c` and `calloc.c`.

`freelist.c` contains functions that are used to maintain the freelist. `block.h` and `block.c` define the structure for the blocks and provide some utility functions for blocks.

`thread-test.c` and `test.c` contain some tests for multi-threaded and single-threaded memory requests.

#### Allocation

Memory is allocated using Buddy Allocation. A normal memory request to the malloc function roughly looks like this.

- Check if `heap` is empty. If so, extend heap by calling `sbrk`.
- Calculate the **level** for the given memory request. For example, a memory request for 200 bytes will be turned into 256 bytes since, all the blocks are in power of 2 in this allocation scheme.
- Once we know the **level**, we check if there are any free blocks in `blocks` for that level.
	- If so, allocate the block and update the free list accordingly.
	- If not, we find the next largest free block in `blocks` and keep breaking it in two until we can satisfy the memory request.
	- If there are no blocks that can be broken down to satisfy the request, we ask for more heap space using the `sbrk` system call and satisfy the request.

Note that, the request made to the sbrk call is multiple of page size.

If the memory request is much bigger than the biggest block available, the memory is satisfied using `mmap` system call.


### Building and Testing

You can issue the command `make check` to test the implementation, which will build `libmalloc.so` shared object and run `thread-test.c` with it.